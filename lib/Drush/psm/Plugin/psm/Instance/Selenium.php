<?php

/**
 * @file
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBasePid;

class Selenium extends InstanceBasePid {

  /**
   * Get the command to start the instance.
   *
   * @return array
   *   Zero based numeric indexed array. The array is suitable for the
   *   _drush_shell_exec().
   */
  protected function getExecutable() {
    // @todo: Implement getExecutable() method.
  }
}
