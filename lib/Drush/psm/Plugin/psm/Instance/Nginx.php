<?php

/**
 * @file
 * Home of the Nginx server instance manager class.
 *
 * @see http://nginx.org
 * @see http://wiki.nginx.org/CommandLine
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

/**
 * Nginx server instance manager.
 */
class Nginx extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected static $executableNames = array(
    'nginx',
  );

  /**
   * {@inheritdoc}
   */
  protected function sendSignal($signal) {
    $arg = NULL;

    switch ($signal) {
      case static::SIGNAL_RELOAD:
        $arg = 'reload';
        break;

      case static::SIGNAL_STOP:
      case static::SIGNAL_QUIT:
        $arg = 'stop';
        break;

    }

    if (!$arg) {
      throw new \Exception("Invalid argument $signal");
    }

    $command = $this->getStartCommand();
    $command->executable .= ' -s ' . $arg;

    return $command->run();
  }

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $default_options = array('p' => '', 'c' => '', 'g' => '');
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    $options += $default_options;
    $options = array_intersect_key($options, $default_options);

    foreach ($options as $option => $value) {
      if ($value) {
        $command->executable .= " -{$option} %s";
        $command->arguments[] = $value;
      }
    }

    return $command;
  }

}
