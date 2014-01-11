<?php

/**
 * @file
 * Home of the PsmInstanceBase class.
 */

abstract class PsmInstanceBase implements PsmInstanceInterface {

  /**
   * Array to store the instances.
   *
   * @var array
   */
  protected static $instances = array();

  /**
   * Seconds to wait before check the status of the instance.
   *
   * @var int
   */
  protected static $defaultStatusDelay = 0;

  /**
   * {@inherit}
   */
  public static function factory(array $info) {
    if (!array_key_exists($info['name'], static::$instances)) {
      static::$instances[$info['name']] = new static($info);
    }

    return static::$instances[$info['name']];
  }

  /**
   * Instance definition.
   *
   * @var array
   */
  protected $info = array();

  /**
   * Command line options after the executable to retrieve an output which is
   * contains the version number.
   *
   * @var string
   */
  protected $versionOption = '-v';

  /**
   * Regular expresion to identifi the version number.
   *
   * @var string
   */
  protected $versionPattern = '/(?P<version>\d[^\s]{0,})/';

  /**
   * Create an instance handler.
   *
   * @param array $info
   *   Instance definition.
   */
  protected function __construct(array $info) {
    $this->info = $info;
  }

  /**
   * Get the instance definition.
   *
   * @return array
   *   Instance definition.
   */
  public function getInfo() {
    return $this->info;
  }

  /**
   * {@inherit}
   */
  public function name() {
    return $this->getInfoEntry('name');
  }

  /**
   * {@inherit}
   */
  public function service() {
    return $this->getInfoEntry('service');
  }

  /**
   * {@inherit}
   */
  public function label() {
    return $this->getInfoEntry('label');
  }

  /**
   * {@inherit}
   */
  public function description() {
    return $this->getInfoEntry('description');
  }

  /**
   * {@inherit}
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
   * Get an entry from the instance definition.
   *
   * @param string $key
   *   Array key.
   * @param bool $required
   *   Indicate the $key must be exists or not.
   * @param mixed $default
   *   If the existence is optional the this will be the default value.
   *
   * @throws Exception
   * @return mixed
   *   Value of the $key.
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
   * Get the amount of delay seconds.
   *
   * @return int
   *   Amount of seconds.
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
   * Get the file path of the PID file.
   *
   * @return string
   *   File system path to PID file.
   */
  public function getPidFile() {
    return $this->getInfoEntry('pid_file');
  }

  /**
   * Get the process ID.
   *
   * @return int
   *   The process ID.
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
   * Check the process is running or not.
   *
   * @return int|bool
   *   The process ID or FALSE.
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
   * Get the executed command.
   *
   * @return string|bool
   *   Command.
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
   * Check the status of the instance by the PID file.
   *
   * @deprecated Use directly the isPidRunning() method.
   *
   * @return int
   *   Process ID.
   */
  protected function statusByPidFile() {
    return $this->isPidRunning();
  }

  /**
   * Get the command to start the instance.
   *
   * @return array
   *   Zero based numeric indexed array. The array is suitable for the
   *   _drush_shell_exec().
   */
  protected abstract function getExecutable();

  /**
   * Basic process handler.
   *
   * @param int $signal
   *   Reload, stop.
   *
   * @return bool|null
   *   Status of the command execution.
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
