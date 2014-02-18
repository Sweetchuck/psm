<?php

/**
 * @file
 * home of the PsmInstanceInterface interface.
 */

namespace Drush\psm;

/**
 * Interface InstanceInterface
 *
 * @package Drush\psm
 */
interface InstanceInterface {

  /**
   * Get instance handler objects.
   *
   * @param array $info
   *   Instance info array.
   *
   * @return InstanceInterface
   *   Instance handler object.
   */
  public static function factory(array $info);

  /**
   * Populate the default values.
   *
   * @param array $info
   *   Instance info array.
   *
   * @return array
   *   Instance info array.
   */
  public static function defaultInfo(array $info);

  /**
   * Get instance definition.
   *
   * @return array
   *   Instance definition.
   */
  public function getInfo();

  /**
   * Get instance name. This is a unique identifier in the service namespace.
   *
   * @return string
   *   Instance name.
   */
  public function name();

  /**
   * Get service name.
   *
   * @return string
   *   Service identifier.
   */
  public function service();

  /**
   * Get the instance ID.
   *
   * @return string
   *   The service name and the instance name separated by dot.
   */
  public function id();

  /**
   * Get the human readable name of the service.
   *
   * @return string
   *   Human readable name of the service.
   */
  public function label();

  /**
   * Get description of the instance.
   *
   * @return string
   *   Description of the instance.
   */
  public function description();

  /**
   * Get the version number of the service.
   *
   * @return string
   *   Version of the service.
   */
  public function version();

  /**
   * Get the status of the instance.
   *
   * @param int|bool $delay
   *   FALSE means no delay.
   *   TRUE means default amount of delay.
   *   Integer value means number of seconds to delay.
   *   Wait $delay seconds before check the status.
   *
   * @return int|bool
   *   If the instance is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function status($delay = FALSE);

  /**
   * Start the instance.
   *
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function start();

  /**
   * Stop the instance.
   *
   * @return bool
   *   TRUE if the service is not running otherwise FALSE.
   */
  public function stop();

  /**
   * Restart the instance.
   *
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function restart();

  /**
   * Reload the configuration of the instance.
   *
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function reload();
}
