<?php

/**
 * @file
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBasePid;

class Selenium extends InstanceBasePid {

  public function version() {
    if ($this->versionNumber === NULL) {
      $this->versionNumber = '';
      $executable = realpath($this->getInfoEntry('jar'));
      $matches = array();
      if (preg_match('/(?P<version>\d.*?)\.jar$/', pathinfo($executable, PATHINFO_BASENAME), $matches)) {
        $this->versionNumber = $matches['version'];
      }
    }

    return $this->versionNumber;
  }

  /**
   * Get the command to start the instance.
   *
   * @return array
   *   Zero based numeric indexed array. The array is suitable for the
   *   _drush_shell_exec().
   */
  protected function getStartCommand() {
    // @todo: Implement getStartCommand() method.
  }
}
