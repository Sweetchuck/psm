<?php

/**
 * @file
 * Home of the MemCache instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

/**
 * MemCache instance manager.
 */
class Memcache extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'memcached',
  );

  /**
   * @var string
   */
  protected $versionOption = '-h';

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $pid_file = $this->getPidFile();
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    $options['-P'] = $pid_file ? $pid_file : FALSE;

    $command->addOptions($options, array(
        'key_value_separator' => ' ',
      ));

    return $command;
  }

}
