<?php

/**
 * @file
 * Home of the PsmInstanceBase class.
 */

abstract class PsmInstanceBase implements PsmInstanceInterface {

  /**
   * @var array
   */
  static protected $instances = array();

  /**
   * @param array $info
   *
   * @return PsmInstanceInterface
   */
  static public function factory(array $info) {
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
  static public function getPid($file_name) {
    if (!$file_name || !is_readable($file_name)) {
      return 0;
    }

    $pid = trim(file_get_contents($file_name));

    return preg_match('/^\d+$/', $pid) ? (int) $pid : 0;
  }

  /**
   * @param int $pid
   *
   * @return string|bool
   */
  static public function getCmdLine($pid) {
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
  protected $versionPattern = '/(?P<version>\d+[^\s]{0,})/';

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
   * @return string
   */
  public function version() {
    $cmd = escapeshellcmd($this->getInfoEntry('executable'));
    $cmd .= ' ' . $this->versionOption;
    if (drush_shell_exec($cmd)) {
      $output = implode("\n", drush_shell_exec_output());
      $matches = array('version' => '');
      preg_match($this->versionPattern, $output, $matches);

      return $matches['version'];
    }

    return '';
  }

  /**
   * @return int
   */
  public function status() {
    $info = $this->getInfo();

    return static::getPid($info['pid_file']);
  }

//  public abstract function start();
//
//  public abstract function stop();
//
//  public abstract function restart();
//
//  public abstract function reload();

  /**
   * @param string $key
   *
   * @return mixed
   *
   * @throws Exception
   */
  protected function getInfoEntry($key) {
    $info = $this->getInfo();

    if (!array_key_exists($key, $info)) {
      throw new Exception('Invalid argument', 1);
    }

    return $info[$key];
  }

}
