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
  protected static $executableNames = array(
    'php-fpm',
  );

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

    $options = $this->getExecutableOptions();
    $pid_file = $this->getPidFile();
    $options['-g'] = $pid_file ? $pid_file : FALSE;
    $command->addOptions($options);

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
      '-c' => '',
      '-n' => FALSE,
      '-d' => '',
      '-e' => FALSE,
      '-p' => '',
      '-g' => FALSE,
      '-y' => '',
      '-D' => FALSE,
      '-F' => FALSE,
      '-R' => FALSE,
    );

    return $options;
  }

}
