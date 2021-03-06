<?php

/**
 * @file
 * Home of the PostgreSQL instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

/**
 * PostgreSQL instance manager.
 */
class Postgresql extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'postgres',
  );

  /**
   * {@inheritdoc}
   */
  protected $versionOption = '--version';

  /**
   * {@inheritdoc}
   */
  public static function defaultInfo(array $info) {
    return array(
      'daemon' => 'nohup',
    ) + parent::defaultInfo($info);
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $command->optionProperties['key_value_separator'] = ' ';
    $command->interactive = TRUE;

    $pid_file = $this->getPidFile();
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    if ($pid_file) {
      $options['-c'] = 'external_pid_file=' . $pid_file;
      $command->pidFile = NULL;
    }

    $command->addOptions($options);

    return $command;
  }

}
