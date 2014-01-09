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
   * @param string $file_name
   *
   * @return int
   */
  public static function getPid($file_name) {
    if (!$file_name || !is_readable($file_name)) {
      return 0;
    }

    $pid = trim(file_get_contents($file_name));

    return preg_match('/^\d+$/', $pid) ? (int) $pid : 0;
  }

  /**
   * @param int $pid
   *   Process ID.
   *
   * @return bool
   */
  public static function isPidRunning($pid) {
    if (!$pid) {
      return FALSE;
    }

    $cmd = sprintf('kill -s 0 %d', $pid);
    $return_var = NULL;
    $output = NULL;
    exec($cmd, $output, $return_var);

    return $return_var === 0;
  }

  /**
   * @param int $pid
   *
   * @return string|bool
   */
  public static function getCmdLine($pid) {
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
  protected function statusByPidFile() {
    $pid = static::getPid($this->getInfoEntry('pid_file'));

    return static::isPidRunning($pid) ? $pid : 0;
  }

  /**
   * @return int
   */
  protected function getStatusDelay() {
    return $this->getInfoEntry('status_delay', FALSE, static::$defaultStatusDelay);
  }

}
