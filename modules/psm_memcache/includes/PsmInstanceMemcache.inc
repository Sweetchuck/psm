<?php

/**
 * @file
 * Home of the MemCache instance manager class.
 */

/**
 * MemCache instance manager.
 */
class PsmInstanceMemcache extends PsmInstanceBasePid {

  /**
   * @var string
   */
  protected $versionOption = '-h';

  /**
   * {@inherit}
   */
  protected function getExecutable() {
    $options = $this->getInfoEntry('executable_options', FALSE, array());
    $cmd = array($this->getInfoEntry('executable'));
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
          $cmd[0] .= " -$option_name %s";
          $cmd[] = $option_value;
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
          $cmd[0] .= " -$option_name";
          break;

        case 'l':
          if (is_array($option_value)) {
            $option_value = implode(',', $option_value);
          }

          $option_value = trim($option_value);

          if ($option_value) {
            $cmd[0] .= " -$option_name %s";
            $cmd[] = $option_value;
          }
          break;


      }
    }

    return $cmd;
  }

}
