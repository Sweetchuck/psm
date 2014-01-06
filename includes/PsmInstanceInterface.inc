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
   * @return int
   */
  public function status();

  public function start();
  public function stop();
//  public function restart();
//  public function reload();
}
