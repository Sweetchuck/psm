<?php

/**
 * @file
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

class Selenium extends InstanceBase {

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
   * {@inheritdoc}
   */
  protected function getStartCommand() {
    // @todo: Implement getStartCommand() method.
  }
}
