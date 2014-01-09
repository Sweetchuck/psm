<?php
/**
 * @file
 *
 */

interface PsmInstanceInterface {

  /**
   * @param array $info
   *
   * @return PsmInstanceInterface
   */
  static function factory(array $info);

  /**
   * @return array
   */
  public function getInfo();

  /**
   * @return string
   */
  public function name();

  /**
   * @return string
   */
  public function service();

  /**
   * @return string
   */
  public function label();

  /**
   * @return string
   */
  public function description();

  /**
   * @return string
   */
  public function version();

  /**
   * @param int|bool $delay
   *   FALSE means no delay.
   *   TRUE means default amount of delay.
   *   Integer value means number of seconds to delay.
   *   Wait $delay seconds before check the status.
   *
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function status($delay = FALSE);

  /**
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function start();

  /**
   * @return bool
   *   TRUE if the service is not running otherwise FALSE.
   */
  public function stop();

  /**
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function restart();

  /**
   * @return int|bool
   *   If the service is running the process ID if available or TRUE otherwise
   *   zero or FALSE.
   */
  public function reload();
}
