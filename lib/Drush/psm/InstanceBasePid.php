<?php

/**
 * @file
 * Home of the InstanceBasePid class.
 */

namespace Drush\psm;

/**
 * Pid based process management.
 *
 * Class PsmInstanceBasePid.
 */
abstract class InstanceBasePid extends InstanceBase {

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

    return $this->isPidRunning();
  }

  /**
   * {@inherit}
   */
  public function start() {
    $status = $this->status();
    if ($status) {
      return $status;
    }

    if (_drush_shell_exec($this->getStartCommand())) {
      return $this->status(TRUE);
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
  protected abstract function getStartCommand();

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
