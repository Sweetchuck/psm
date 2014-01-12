<?php

/**
 * @file
 * Home of the InstanceBase class.
 */

namespace Drush\psm;

/**
 * Class InstanceBase.
 *
 * @package Drush\psm
 */
abstract class InstanceBase implements InstanceInterface {

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
