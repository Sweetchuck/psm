<?php

/**
 * @file
 * Home of the Php-Fpm instance manager class.
 */

/**
 * PhpFpm instance manager.
 */
class PsmInstancePhpfpm extends PsmInstanceBase {

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

    $pid_file = $this->getInfoEntry('pid_file');
    $cmd = array($this->getInfoEntry('executable'));
    $options = $this->getExecutableOptions();

    foreach ($options as $option => $value) {
      switch ($option) {
        case 'n':
        case 'e':
        case 'D':
        case 'F':
        case 'R':
          if ($value) {
            $cmd[0] .= " -$option";
          }
          break;

        case 'c':
        case 'p':
        case 'y':
          if ($value) {
            $cmd[0] .= " -$option %s";
            $cmd[] = $value;
          }
          break;

        case 'g':
          if ($value) {
            $cmd[0] .= ' -g %s';
            $cmd[] = $pid_file;
          }
          break;

      }
    }

    if (_drush_shell_exec($cmd)) {
      return $this->status(TRUE);
    }

    return 0;
  }

  /**
   * {@inherit}
   */
  public function stop() {
    if (!$this->status()) {
      return TRUE;
    }

    $cmd = 'kill -s 15 %d';
    $pid_file = $this->getInfoEntry('pid_file');
    $pid = static::getPid($pid_file);
    if (drush_shell_exec($cmd, $pid)) {
      return !$this->status(TRUE);
    }

    return FALSE;
  }

  /**
   * {@inherit}
   */
  public function reload() {
    if ($this->status()) {
      $cmd = 'kill -USR2 %d';
      $pid = static::getPid($this->getInfoEntry('pid_file'));
      if (drush_shell_exec($cmd, $pid)) {
        return $this->status(TRUE);
      }
    }

    return FALSE;
  }

  protected function getExecutableOptions() {
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    $options += array(
      'c' => '',
      'n' => FALSE,
      'd' => '',
      'e' => FALSE,
      'p' => '',
      'g' => FALSE,
      'y' => '',
      'D' => FALSE,
      'F' => FALSE,
      'R' => FALSE,
    );

    return $options;
  }

}
