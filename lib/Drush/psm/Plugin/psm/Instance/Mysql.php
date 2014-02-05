<?php

/**
 * @file
 * Home of the MySQL instance handler class.
 */

namespace Drush\psm\Plugin\psm\Instance;

use Drush\psm\InstanceBase;

/**
 * Mysql instance handler class.
 */
class Mysql extends InstanceBase {

  /**
   * {@inheritdoc}
   */
  protected $versionOption = '--help';

  /**
   * {@inheritdoc}
   */
  protected $versionPattern = '/(?<=Ver )(?P<version>\d[^\s]{0,})/';

}
