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
class PsmInstanceNginx extends PsmInstanceBase {

  /**
   * Configuration reload.
   *
   * Start the new worker processes with a new configuration.
   * Gracefully shutdown the old worker processes.
   */
  const SIGNAL_RELOAD = 'reload';

  /**
   * Will stop the Nginx server.
   */
  const SIGNAL_STOP = 'stop';

  /**
   * Graceful shutdown.
   */
  const SIGNAL_QUIT = 'quit';

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

  protected function sendSignal($signal) {
    return drush_shell_exec($this->getExecutable() . ' -s ' . $signal);
  }

  protected function getExecutable() {
    $option2alias = array(
      'p' => 'prefix',
      'c' => 'config_file',
      'g' => 'directives',
    );

    $alias2value = $this->getInfoEntry('executable_options', FALSE, array());
    $alias2value += array_fill_keys($option2alias, '');

    $cmd = array($this->getInfoEntry('executable'));
    foreach ($option2alias as $option => $alias) {
      if ($alias2value[$alias]) {
        $cmd[0] .= " -{$option} %s";
        $cmd[] = $alias2value[$alias];
      }
    }

    return $cmd;
  }

}
