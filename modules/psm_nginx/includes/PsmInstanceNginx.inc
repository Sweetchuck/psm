<?php

/**
 * @file
 * Home of the Nginx server instance manager class.
 *
 * @see http://nginx.org
 * @see http://wiki.nginx.org/CommandLine
 */

/**
 * Nginx server instance manager.
 */
class PsmInstanceNginx extends PsmInstanceBasePid {

  /**
   * @param int $signal
   *
   * @return bool|null
   *
   * @throws Exception
   */
  protected function sendSignal($signal) {
    $arg = NULL;

    switch ($signal) {
      case static::SIGNAL_RELOAD:
        $arg = 'reload';
        break;

      case static::SIGNAL_STOP:
      case static::SIGNAL_QUIT:
        $arg = 'stop';
        break;

    }

    if (!$arg) {
      throw new Exception("Invalid argument $signal");
    }

    return drush_shell_exec($this->getExecutable() . ' -s ' . $arg);
  }

  /**
   * @return array
   */
  protected function getExecutable() {
    $default_options = array('p' => '', 'c' => '', 'g' => '');
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    $options += $default_options;
    $options = array_intersect_key($options, $default_options);

    $cmd = array($this->getInfoEntry('executable'));
    foreach ($options as $option => $value) {
      if ($value) {
        $cmd[0] .= " -{$option} %s";
        $cmd[] = $value;
      }
    }

    return $cmd;
  }

}
