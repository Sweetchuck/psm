<?php

/**
 * @file
 * Drush integration of the Personal Service Manager.
 */

/**
 * Implements hook_drush_command().
 */
function psm_drush_command() {
  $commands = array();

  $commands['docs-psm'] = array(
    'description' => dt('Personal service manager.'),
    'hidden' => TRUE,
    'topic' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'callback' => 'drush_print_file',
    'callback arguments' => array(__DIR__ . '/README.md'),
  );

  $commands['psm-services'] = array(
    'aliases' => array('psm'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Something helpful description.'),
    'required-arguments' => FALSE,
    'arguments' => array(
      'service' => dt('Service'),
    ),
    'examples' => array(
      'drush psm-services' => dt('Description of what this command will do.'),
    ),
    'engines' => array(
      'outputformat' => array(
        'default' => 'table',
        'pipe-format' => 'list',
        'field-labels' => array(
          'name' => dt('Name'),
          'label' => dt('Label'),
          'description' => dt('Description'),
          'links' => dt('Links'),
        ),
        'output-data-type' => 'format-table',
      ),
    ),
  );

  $commands['psm-instances'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Something helpful description.'),
    'required-arguments' => FALSE,
    'arguments' => array(
      'service' => dt('Service'),
      'instance' => dt('Instance'),
    ),
    'options' => array(
      'group' => array(
        'description' => dt('Group name. Only used if the instance argument is omitted.'),
        'example_value' => 'my-project-01',
        'required' => FALSE,
        'hidden' => FALSE,
      ),
    ),
    'examples' => array(
      'drush psm-instances' => dt('Description of what this command will do.'),
      'drush psm-instances memcache' => dt('Description of what this command will do.'),
      'drush psm-instances memcache 3306' => dt('Description of what this command will do.'),
    ),
    'engines' => array(
      'outputformat' => array(
        'default' => 'table',
        'pipe-format' => 'list',
        'field-labels' => array(
          'service' => dt('Service'),
          'name' => dt('Name'),
          'label' => dt('Label'),
          'description' => dt('Description'),
          'status' => dt('Status'),
          'version' => dt('Version'),
          'uri' => dt('URI'),
        ),
        'output-data-type' => 'format-table',
      ),
    ),
  );

  $commands['psm-groups'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Something helpful description.'),
    'required-arguments' => FALSE,
    'arguments' => array(
      'group' => dt('Group name'),
    ),
    'examples' => array(
      'drush psm-groups' => dt('@todo'),
      'drush psm-groups my-project-01' => dt('@todo'),
    ),
    'engines' => array(
      'outputformat' => array(
        'default' => 'table',
        'pipe-format' => 'list',
        'field-labels' => array(
          'name' => dt('Name'),
          'label' => dt('Label'),
          'description' => dt('Description'),
        ),
        'output-data-type' => 'format-table',
      ),
    ),
  );

  $base = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'required-arguments' => FALSE,
    'arguments' => array(
      'service' => dt('Name of the service.'),
      'instance' => dt('Name of the service.'),
    ),
    'options' => array(
      'group' => array(
        'description' => dt('Group name. Only used if no argument passed.'),
        'example_value' => 'my-project-01',
        'required' => FALSE,
        'hidden' => FALSE,
      ),
    ),
  );

  $commands['psm-status'] = array(
    'description' => dt('Something helpful description.'),
    'examples' => array(
      'drush psm-status' => dt('Status messages about the all available services and instances.'),
      'drush psm-status memcache' => dt('Status messages about the all instances of the memcache service.'),
      'drush psm-status memcache 11211' => dt('Status messages of the memcache service which is running on the port 11211.'),
    ),
  ) + $base;

  $commands['psm-start'] = array(
    'description' => dt('Something helpful description.'),
    'examples' => array(
      'drush psm-start' => dt('Start the all available instances of all available services.'),
      'drush psm-start memcache' => dt('@todo'),
      'drush psm-start memcache 11211' => dt('@todo.'),
    ),
  ) + $base;

  $commands['psm-stop'] = array(
    'description' => dt('Something helpful description.'),
    'examples' => array(
      'drush psm-stop' => dt('Stop the all available instances of all available services.'),
      'drush psm-stop memcache' => dt('@todo'),
      'drush psm-stop memcache 11211' => dt('@todo.'),
    ),
  ) + $base;

  $commands['psm-restart'] = array(
    'description' => dt('Something helpful description.'),
    'examples' => array(
      'drush psm-restart' => dt('Restart the all available instances of all available services.'),
      'drush psm-restart memcache' => dt('@todo'),
      'drush psm-restart memcache 11211' => dt('@todo.'),
    ),
  ) + $base;

  $commands['psm-reload'] = array(
    'description' => dt('Something helpful description.'),
    'examples' => array(
      'drush psm-reload' => dt('Restart the all available instances of all available services.'),
      'drush psm-reload memcache' => dt('@todo'),
      'drush psm-reload memcache 11211' => dt('@todo.'),
    ),
  ) + $base;

  return $commands;
}

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_services_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_services_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_services_validate($name = NULL) {
  if ($name && !array_key_exists($name, psm_info_service())) {
    drush_set_error('psm_unknown_service', dt('Service "@name" is not exists.', array('@name' => $name)));
  }
}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_services($name = NULL) {
  $services = psm_info_service();

  if ($name) {
    $services = array($name => $services[$name]);
  }

  $output = array();
  foreach ($services as $service_name => $service) {
    $output[$service_name] = array(
      'name' => $service['name'],
      'label' => $service['label'],
      'description' => $service['description'],
      'links' => implode(' ', $service['links']),
    );
  }

  return $output;
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_instances_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_instances_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_instances_validate($service_name = NULL, $instance_name = NULL) {
  if ($service_name) {
    if (!array_key_exists($service_name, psm_info_service())) {
      drush_set_error('psm_unknown_service', dt('Service "@name" is not exists.', array('@name' => $service_name)));
    }
    elseif ($instance_name) {
      $instances = psm_info_instance();
      if (array_key_exists($service_name, $instances) && !array_key_exists($instance_name, $instances[$service_name])) {
        drush_set_error('psm_unknown_instance', dt(
          'Instance "@service:@instance" is not exists.',
          array(
            '@service' => $service_name,
            '@instance' => $instance_name,
          )
        ));
      }
    }
  }

  $group_name = drush_get_option('group');
  if ($group_name) {
    if ($service_name) {
      drush_log(dt('The group option is ignored because the service name is specified'), 'warning');
    }

    $groups = psm_info_group();
    if (!array_key_exists($group_name, $groups)) {
      drush_set_error('psm_unknown_group', dt('Group "@name" is not exists.', array('@name' => $group_name)));
    }
  }
}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_instances($service_name = NULL, $instance_name = NULL) {
  $instances = psm_info_instance();

  $group_name = drush_get_option('group', NULL);
  if (!$service_name && !$instance_name && $group_name) {
    $tmp_instances = array();
    $groups = psm_info_group();
    foreach ($groups[$group_name]['instances'] as $group_service_name => $group_instances) {
      foreach ($group_instances as $group_instance) {
        $tmp_instances[$group_service_name][$group_instance] = $instances[$group_service_name][$group_instance];
      }
    }
    $instances = $tmp_instances;
  }
  elseif ($service_name && $instance_name) {
    $instances = array(
      $service_name => array(
        $instance_name => $instances[$service_name][$instance_name],
      ),
    );
  }
  elseif ($service_name) {
    $instances = array(
      $service_name => $instances[$service_name],
    );
  }

  $services = psm_info_service();

  $output = array();
  foreach ($instances as $service_name => $instance_list) {
    $service = $services[$service_name];
    foreach ($instance_list as $name => $instance) {
      $output[$name] = array(
        'service' => $service['label'],
        'name' => $instance['name'],
        'label' => $instance['label'],
        'description' => $instance['description'],
        'status' => $instance['status'],
        'version' => $instance['version'],
        'uri' => $instance['uri'],
      );
    }
  }

  return $output;
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_status_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_status_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
//function drush_psm_status_validate() {}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_status() {
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_start_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_start_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
//function drush_psm_start_validate() {}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_start() {
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_stop_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_stop_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
//function drush_psm_stop_validate() {}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_stop() {
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_reload_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_reload_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
//function drush_psm_reload_validate() {}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_reload() {
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_restart_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_restart_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
//function drush_psm_restart_validate() {}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_restart() {
}

//drush_hook_post_COMMAND

/**
 * Implements drush_COMMAND_init().
 */
//function drush_psm_groups_init() {}

/**
 * Implements drush_COMMAND_pre_validate().
 */
//function drush_psm_groups_pre_validate() {}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_groups_validate($name = NULL) {
  if ($name && !array_key_exists($name, psm_info_group())) {
    drush_set_error('psm_unknown_group', dt('Group "@name" is not exists.', array('@name' => $name)));
  }
}

//drush_hook_pre_COMMAND

/**
 * Implements drush_COMMAND().
 */
function drush_psm_groups($name = NULL) {
  $groups = psm_info_group();

  if ($name) {
    $groups = array($name => $groups[$name]);
  }

  $output = array();
  foreach ($groups as $group_name => $group) {
    $output[$group_name] = array(
      'name' => $group['name'],
      'label' => $group['label'],
      'description' => $group['description'],
    );
  }

  return $output;
}

//drush_hook_post_COMMAND

/**
 * @return array
 */
function psm_info_service() {
  static $services = NULL;

  if ($services === NULL) {
    $services = drush_command_invoke_all('drush_psm_service_info');

    $command_files = drush_commandfile_list();
    foreach (array_keys($services) as $service_name) {
      $service =& $services[$service_name];

      $service = array_replace_recursive(
        _psm_info_service_defaults($service_name, $service),
        $service
      );
      if (
        $service['class']
        && !$service['files']
        && $service['command_file']
      ) {
        $command_file_dir = dirname($command_files[$service['command_file']]);
        $file = "$command_file_dir/includes/{$service['class']}.inc";
        if (file_exists($file)) {
          $service['files'][] = $file;
        }
      }
    }

    drush_command_invoke_all('drush_psm_service_info_alter', $services);

    foreach (array_keys($services) as $service_name) {
      if (!_psm_class_exists($services[$service_name]['class'], $services[$service_name]['files'])) {
        unset($services[$services]);
        continue;
      }

      $services[$service_name]['name'] = $service_name;
    }
  }

  return $services;
}

/**
 * @return array
 */
function psm_info_instance() {
  static $instances = NULL;

  if ($instances === NULL) {
    $instances = _psm_config('instances');
    foreach (array_keys($instances) as $service_name) {
      foreach (array_keys($instances[$service_name]) as $instance_name) {
        $instances[$service_name][$instance_name]['name'] = $instance_name;
      }
    }
  }

  return $instances;
}

/**
 * @return array
 */
function psm_info_group() {
  static $groups = NULL;

  if ($groups === NULL) {
    $groups = _psm_config('groups');
    foreach (array_keys($groups) as $group_name) {
      $group =& $groups[$group_name];
      $group['name'] = $group_name;
    }
  }

  return $groups;
}

/**
 * @param string $service_name
 * @param array $info
 *
 * @return array
 */
function _psm_info_service_defaults($service_name, $info) {
  $base = "psm_{$service_name}_drush_psm_service";

  return array(
    // Provider module.
    // Required.
    'command_file' => "psm_$service_name",
    // Machine name of the service.
    // Required, but force populated later.
    'name' => $service_name,
    // Human name of the service
    // Required.
    'label' => $service_name,
    // Long description.
    // Optional.
    'description' => NULL,

    'files' => array(),
    'class' => 'PsmService' . strtoupper(substr($service_name, 0, 1)) . substr($service_name, 1),
    // Prefix of the callback functions.
    'base' => $base,
    // Callback function names.
    'callback' => array(
      'instance' => "{$base}_instance",
    ),
  );
}

/**
 * @param string $class
 * @param array $files
 *
 * @return bool
 */
function _psm_class_exists($class, $files = array()) {
  if (class_exists($class, FALSE)) {
    return TRUE;
  }

  foreach ($files as $file) {
    if (file_exists($file)) {
      @include_once($file);
    }
  }

  return class_exists($class, FALSE);
}

/**
 * @param string $subject
 *
 * @return array
 */
function _psm_config($subject) {
  $return = array();

  $files = _psm_config_files($subject);
  foreach ($files as $file) {
    $$subject = NULL;
    $include_result = @include_once($file);
    if ($include_result === FALSE) {
      drush_log(dt('Cannot open drushrc "!config", ignoring.', array('!config' => realpath($file))), 'warning');
      continue;
    }
    elseif (!is_array($$subject)) {
      drush_log(dt('@variable is not an array.', array('@variable' => $subject)), 'warning');
      continue;
    }
    elseif (empty($$subject)) {
      continue;
    }

    $return = $$subject + $return;
  }

  return $return;
}

/**
 * @param string $subject
 * @param string[]|null $paths
 *
 * @return string[]
 */
function _psm_config_files($subject, $paths = NULL) {
  if ($paths === NULL) {
    $paths = _psm_config_paths($subject);
  }

  $patterns = array(
    '/^' . preg_quote($subject, '/') . '\.psm\.drushrc\.php$/',
  );

  $files = array();
  foreach ($paths as $path) {
    foreach ($patterns as $pattern) {
      $files = array_merge(
        $files,
        array_keys(drush_scan_directory($path, $pattern))
      );
    }
  }

  return $files;
}

/**
 * @param string $subject
 *
 * @return string[]
 */
function _psm_config_paths($subject) {
  return (array) drush_get_context('ALIAS_PATH', array());
}
