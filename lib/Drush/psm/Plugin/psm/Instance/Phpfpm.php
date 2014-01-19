<?php

/**
 * @file
 * Home of the Php-Fpm instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

/**
 * PhpFpm instance manager.
 */
class Phpfpm extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  public function stop() {
    $pid = $this->status();
    if (!$pid) {
      return TRUE;
    }

    if (drush_shell_exec('kill -s 15 %d', $pid)) {
      return !$this->status(TRUE);
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function reload() {
    $pid = $this->status();
    if ($pid) {
      if (drush_shell_exec('kill -USR2 %d', $pid)) {
        return $this->status(TRUE);
      }
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    foreach ($this->getExecutableOptions() as $option => $value) {
      switch ($option) {
        case 'n':
        case 'e':
        case 'D':
        case 'F':
        case 'R':
          if ($value) {
            $command->executable .= " -$option";
          }
          break;

        case 'c':
        case 'p':
        case 'y':
          if ($value) {
            $command->executable .= " -$option %s";
            $command->arguments[] = $value;
          }
          break;

        case 'g':
          if ($value) {
            $command->executable .= ' -g %s';
            $command->arguments[] = $this->getPidFile();
          }
          break;

      }
    }

    return $command;
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
