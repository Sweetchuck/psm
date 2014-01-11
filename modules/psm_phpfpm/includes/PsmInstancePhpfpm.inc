<?php

/**
 * @file
 * Home of the Php-Fpm instance manager class.
 */

/**
 * PhpFpm instance manager.
 */
class PsmInstancePhpfpm extends PsmInstanceBasePid {

  /**
   * {@inherit}
   */
  public function stop() {
    if (!$this->status()) {
      return TRUE;
    }

    if (drush_shell_exec('kill -s 15 %d', static::getPid())) {
      return !$this->status(TRUE);
    }

    return FALSE;
  }

  /**
   * {@inherit}
   */
  public function reload() {
    if ($this->status()) {
      if (drush_shell_exec('kill -USR2 %d', static::getPid())) {
        return $this->status(TRUE);
      }
    }

    return FALSE;
  }

  /**
   * {@inherit}
   */
  protected function getExecutable() {
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
            $cmd[] = $this->getPidFile();
          }
          break;

      }
    }

    return $cmd;
  }

  /**
   * Get the executable options with default values.
   *
   * @return array
   *   Array of command options.
   */
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
