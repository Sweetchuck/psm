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
   * @var string
   */
  protected $versionOption = '-h';

  /**
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    $command = parent::getStartCommand();

    $options = $this->getInfoEntry('executable_options', FALSE, array());
    foreach ($options as $option_name => $option_value) {
      // Special handling of the pid file.
      if ($option_name === 'P' && $option_value) {
        $option_value = $this->getInfoEntry('pid_file');
      }

      if ($option_value === FALSE || $option_value === NULL) {
        continue;
      }

      switch ($option_name) {
        // Key-value.
        case 'p':
        case 'U':
        case 's':
        case 'a':
        case 'u':
        case 'm':
        case 'c':
        case 'P':
        case 'f':
        case 'n':
        case 'D':
        case 't':
        case 'R':
        case 'B':
        case 'I':
        case 'o':
          $command->executable .= " -$option_name %s";
          $command->arguments[] = $option_value;
          break;

        // Flag.
        case 'd':
        case 'r':
        case 'M':
        case 'k':
        case 'v':
        case 'vv':
        case 'vvv':
        case 'L':
        case 'C':
        case 'S':
          $command->executable .= " -$option_name";
          break;

        case 'l':
          if (is_array($option_value)) {
            $option_value = implode(',', $option_value);
          }

          $option_value = trim($option_value);

          if ($option_value) {
            $command->executable .= " -$option_name %s";
            $command->arguments[] = $option_value;
          }
          break;

      }
    }

    return $command;
  }

}
