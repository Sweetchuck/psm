<?php

/**
 * @file
 * Home of the Redis instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

/**
 * Redis instance manager.
 */
class Redis extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'redis-server',
  );

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $command->optionProperties['key_value_separator'] = ' ';

    $command->executable .= ' %s';
    $command->arguments[] = $this->getInfoEntry('config_file');

    $options = $this->getInfoEntry('executable_options', FALSE, array());
    if ($options) {
      $command->addOptions($options);
    }

    return $command;
  }

}
