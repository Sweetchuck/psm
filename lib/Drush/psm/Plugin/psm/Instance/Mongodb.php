<?php

/**
 * @file
 * Home of the MongoDB instance manager class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\Command;
use Drush\psm\InstanceBase;

/**
 * Redis instance manager.
 */
class Mongodb extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'mongod',
  );

  /**
   * {@inheritdoc}
   */
  protected $versionOption = '--version';

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $command->optionProperties['key_value_separator'] = ' ';
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    if ($options) {
      $command->addOptions($options);
    }

    return $command;
  }

  /**
   * {@inheritdoc}
   */
  protected function getStopCommand() {
    $command = new Command();

    $command->executable = $this->getInfoEntry('executable');
    $command->optionProperties['key_value_separator'] = ' ';

    $options = array();

    $all_options = $this->getInfoEntry('executable_options', FALSE, array());
    if (!empty($all_options['--config'])) {
      $options['--config'] = $all_options['--config'];
    }

    $options['--shutdown'] = TRUE;

    $command->addOptions($options);

    return $command;
  }

}
