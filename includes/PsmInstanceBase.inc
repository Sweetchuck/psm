<?php

/**
 * @file
 * Home of the PsmInstanceBase class.
 */

abstract class PsmInstanceBase implements PsmInstanceInterface {

  /**
   * @var array
   */
  protected static $instances = array();

  /**
   * @var int
   */
  protected static $defaultStatusDelay = 0;

  /**
   * @param array $info
   *
   * @return PsmInstanceInterface
   */
  public static function factory(array $info) {
    if (!array_key_exists($info['name'], static::$instances)) {
      static::$instances[$info['name']] = new static($info);
    }

    return static::$instances[$info['name']];
  }

  /**
   * @var array
   */
  protected $info = array();

  /**
   * @var string
   */
  protected $versionOption = '-v';

  /**
   * @var string
   */
  protected $versionPattern = '/(?P<version>\d[^\s]{0,})/';

  /**
   * @param array $info
   */
  protected function __construct(array $info) {
    $this->info = $info;
  }

  /**
   * @return array
   */
  public function getInfo() {
    return $this->info;
  }

  /**
   * @return string
   */
  public function name() {
    return $this->getInfoEntry('name');
  }

  /**
   * @return string
   */
  public function service() {
    return $this->getInfoEntry('service');
  }

  /**
   * @return string
   */
  public function label() {
    return $this->getInfoEntry('label');
  }

  /**
   * @return string
   */
  public function description() {
    return $this->getInfoEntry('description');
  }

  /**
   * @throws Exception
   * @return string
   */
  public function version() {
    $cmd = escapeshellcmd($this->getInfoEntry('executable', FALSE));
    if ($cmd && $this->versionOption !== NULL) {
      if (!is_executable($cmd)) {
        throw new Exception('Not executable: ' . $cmd, 1);
      }

      $cmd .= ' ' . $this->versionOption;
      if (drush_shell_exec($cmd)) {
        $output = implode("\n", drush_shell_exec_output());
        $matches = array('version' => '');
        preg_match($this->versionPattern, $output, $matches);

        return $matches['version'];
      }
    }

    return '';
  }

  /**
   * {@inherit}
   */
  public abstract function status($delay = FALSE);

  /**
   * {@inherit}
   */
  public abstract function start();

  /**
   * {@inherit}
   */
  public abstract function stop();

  /**
   * {@inherit}
   */
  public function restart() {
    $this->stop();

    return $this->start();
  }

  /**
   * {@inherit}
   */
  public abstract function reload();

  /**
   * @param string $key
   * @param bool $required
   * @param mixed $default
   *
   * @throws Exception
   * @return mixed
   */
  protected function getInfoEntry($key, $required = TRUE, $default = NULL) {
    $info = $this->getInfo();

    if (!array_key_exists($key, $info)) {
      if ($required) {
        throw new Exception("Invalid argument: $key", 1);
      }
      else {
        return $default;
      }
    }

    return $info[$key];
  }

  /**
   * @return int
   */
  protected function getStatusDelay() {
    return $this->getInfoEntry('status_delay', FALSE, static::$defaultStatusDelay);
  }

}

/**
 * Pid based process management.
 *
 * Class PsmInstanceBasePid.
 */
abstract class PsmInstanceBasePid extends PsmInstanceBase {

  /**
   * Configuration reload.
   *
   * Start the new worker processes with a new configuration.
   * Gracefully shutdown the old worker processes.
   */
  const SIGNAL_RELOAD = 1;

  /**
   * Will stop the Nginx server.
   */
  const SIGNAL_STOP = 2;

  /**
   * Graceful shutdown.
   */
  const SIGNAL_QUIT = 3;

  /**
   * {@inherit}
   */
  public function status($delay = FALSE) {
    if ($delay === TRUE) {
      $delay = $this->getStatusDelay();
    }

    if ($delay) {
      sleep((int) $delay);
    }

    return $this->statusByPidFile();
  }

  /**
   * {@inherit}
   */
  public function start() {
    $status = $this->status();
    if ($status) {
      return $status;
    }

    if (_drush_shell_exec($this->getExecutable())) {
      return  $this->status(TRUE);
    }

    return FALSE;
  }

  /**
   * {@inherit}
   */
  public function stop() {
    if (!$this->status()) {
      return TRUE;
    }

    return $this->sendSignal(static::SIGNAL_STOP) ?
      !$this->status(TRUE) : FALSE;
  }

  /**
   * {@inherit}
   */
  public function reload() {
    if (!$this->status()) {
      return FALSE;
    }

    return $this->sendSignal(static::SIGNAL_RELOAD) ?
      $this->status(TRUE) : FALSE;
  }

  /**
   * @return string
   */
  public function getPidFile() {
    return $this->getInfoEntry('pid_file');
  }

  /**
   * @return int
   */
  public function getPid() {
    $pid_file = $this->getPidFile();
    if (!$pid_file || !is_readable($pid_file)) {
      return 0;
    }

    $pid = trim(file_get_contents($pid_file));

    return preg_match('/^\d+$/', $pid) ? (int) $pid : 0;
  }

  /**
   * @return int|bool
   */
  public function isPidRunning() {
    $pid = $this->getPid();
    if (!$pid) {
      return FALSE;
    }

    $cmd = sprintf('kill -s 0 %d', $pid);
    $return_var = NULL;
    $output = NULL;
    exec($cmd, $output, $return_var);

    return $return_var === 0 ? $pid : FALSE;
  }

  /**
   * @return string|bool
   */
  public function getCmdLine() {
    $pid = $this->getPid();
    if (!$pid) {
      return FALSE;
    }

    $file_name = "/proc/$pid/cmdline";
    if (!is_readable($file_name)) {
      return FALSE;
    }

    return (drush_shell_exec("cat $file_name")) ?
      implode("\n", drush_shell_exec_output()) : FALSE;
  }

  /**
   * @return int
   */
  protected function statusByPidFile() {
    return $this->isPidRunning();
  }

  /**
   * @return array
   */
  protected abstract function getExecutable();

  /**
   * @param int $signal
   *
   * @return bool|null
   */
  protected function sendSignal($signal) {
    $executable = $this->getInfoEntry('executable');
    $pid_file = $this->getPidFile();
    $result = NULL;
    switch ($signal) {
      case static::SIGNAL_RELOAD:
        // @todo Configurable path to killproc.
        $result = drush_shell_exec('/sbin/killproc -HUP -p %s %s', $pid_file, $executable);
        break;

      case static::SIGNAL_QUIT:
      case static::SIGNAL_STOP:
        $result = drush_shell_exec('/sbin/killproc -TERM -p %s %s', $pid_file, $executable);
        break;

    }

    return $result;
  }

}
