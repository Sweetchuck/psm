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
    'engines' => array(
      'outputformat' => array(
        'default' => 'table',
        'pipe-format' => 'list',
        'field-labels' => array(
          'service' => dt('Service'),
          'name' => dt('Name'),
          'status' => dt('Status'),
        ),
        'output-data-type' => 'format-table',
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
 * Implements hook_drush_help().
 */
function psm_drush_help($subject = NULL) {
  $subject_args = explode(':', $subject);
  $subject = array_shift($subject_args);
  if ($subject_args) {
    $subject .= ':' . array_shift($subject_args);
  }

  $help = array();
  switch ($subject) {
    case 'error:psm_autoload_missing':
      $help[] = dt('Failed to load the autoloader.');
      $command_files = drush_commandfile_list();
      $help[] = dt('Try to run the `./init.sh` in the "@dir" directory.', array(
          '@dir' => dirname($command_files['psm']),
        ));
      break;

    case 'error:psm_service_unknown':
      $help[] = dt('Service "@name" is not exists.', array('@name' => $subject_args[0]));
      $help[] = dt('Try to run the `drush psm-services`.');
      break;

    case 'error:psm_instance_unknown':
      $help[] = dt('Instance "@service:@instance" is not exists.', array(
        '@service' => $subject_args[0],
        '@instance' => $subject_args[1],
      ));
      $help[] = dt('Try to run the `drush psm-instances`.');
      break;

    case 'error:psm_group_unknown':
      $help[] = dt('Group "@name" is not exists.', array(
        '@name' => $subject_args[0],
      ));
      $help[] = dt('Try to run the `drush psm-groups`.');
      break;

    case 'error:psm_instance_start':
      $help[] = dt('Service @service:@instance failed to start.', array(
        '@service' => $subject_args[0],
        '@instance' => $subject_args[1],
      ));
      break;

    case 'error:psm_instance_stop':
      $help[] = dt('Service @service:@instance failed to stop.', array(
        '@service' => $subject_args[0],
        '@instance' => $subject_args[1],
      ));
      break;

    case 'error:psm_instance_reload':
      $help[] = dt('Service @service:@instance failed to reload.', array(
        '@service' => $subject_args[0],
        '@instance' => $subject_args[1],
      ));
      break;

    case 'error:psm_instance_restart':
      $help[] = dt('Service @service:@instance failed to restart.', array(
        '@service' => $subject_args[0],
        '@instance' => $subject_args[1],
      ));
      break;

  }

  return implode("\n", $help);
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_services_validate($name = NULL) {
  if ($name && !array_key_exists($name, psm_info_service())) {
    drush_set_error("psm_service_unknown:$name");
  }
}

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

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_instances_validate($service_name = NULL, $instance_name = NULL) {
  if ($service_name) {
    if (!array_key_exists($service_name, psm_info_service())) {
      drush_set_error("psm_service_unknown:$service_name");
    }
    elseif ($instance_name) {
      $instances = psm_info_instance();
      if (array_key_exists($service_name, $instances) && !array_key_exists($instance_name, $instances[$service_name])) {
        drush_set_error("psm_instance_unknown:$service_name:$instance_name");
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
      drush_set_error("psm_group_unknown:$group_name");
    }
  }
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_instances($service_name = NULL, $instance_name = NULL) {
  $group_name = drush_get_option('group', NULL);

  $instances = psm_get_instances($service_name, $instance_name, $group_name);
  $services = psm_info_service();

  $output = array();
  foreach ($instances as $service_name => $instance_list) {
    $service = $services[$service_name];
    foreach ($instance_list as $name => $info) {
      $instance = psm_instance_factory($info);
      $output[$name] = array(
        'service' => $service['label'],
        'name' => $instance->name(),
        'label' => $instance->label(),
        'description' => $instance->description(),
        'status' => $instance->status(),
        'version' => $instance->version(),
        'uri' => '',
      );
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_status_validate($service_name = NULL, $instance_name = NULL) {
  drush_psm_instances_validate($service_name, $instance_name);
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_status($service_name = NULL, $instance_name = NULL) {
  $group_name = drush_get_option('group', NULL);

  $instances = psm_get_instances($service_name, $instance_name, $group_name);
  $output = array();
  foreach ($instances as $instance_list) {
    foreach ($instance_list as $info) {
      $instance = psm_instance_factory($info);
      $output[$instance->id()] = array(
        'service' => $instance->service(),
        'name' => $instance->name(),
        'status' => $instance->status(),
      );
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_start_validate($service_name = NULL, $instance_name = NULL) {
  drush_psm_instances_validate($service_name, $instance_name);
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_start($service_name = NULL, $instance_name = NULL) {
  $group_name = drush_get_option('group', NULL);

  $instances = psm_get_instances($service_name, $instance_name, $group_name);
  $output = array();
  foreach ($instances as $instance_list) {
    foreach ($instance_list as $info) {
      $instance = psm_instance_factory($info);
      $id = $instance->id();
      $output[$id] = array(
        'service' => $instance->service(),
        'name' => $instance->name(),
        'status' => $instance->start(),
      );

      if (!$output[$id]['status']) {
        drush_set_error("psm_instance_start:{$output[$id]['service']}:{$output[$id]['name']}");
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_stop_validate($service_name = NULL, $instance_name = NULL) {
  drush_psm_instances_validate($service_name, $instance_name);
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_stop($service_name = NULL, $instance_name = NULL) {
  $group_name = drush_get_option('group', NULL);

  $instances = psm_get_instances($service_name, $instance_name, $group_name);
  $output = array();
  foreach ($instances as $instance_list) {
    foreach ($instance_list as $info) {
      $instance = psm_instance_factory($info);
      $id = $instance->id();
      $output[$id] = array(
        'service' => $instance->service(),
        'name' => $instance->name(),
        'status' => !$instance->stop(),
      );

      if ($output[$id]['status']) {
        drush_set_error("psm_instance_stop:{$output[$id]['service']}:{$output[$id]['name']}");
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_reload_validate($service_name = NULL, $instance_name = NULL) {
  drush_psm_instances_validate($service_name, $instance_name);
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_reload($service_name = NULL, $instance_name = NULL) {
  $group_name = drush_get_option('group', NULL);

  $instances = psm_get_instances($service_name, $instance_name, $group_name);
  $output = array();
  foreach ($instances as $instance_list) {
    foreach ($instance_list as $info) {
      $instance = psm_instance_factory($info);
      $id = $instance->id();
      $output[$id] = array(
        'service' => $instance->service(),
        'name' => $instance->name(),
        'status' => $instance->reload(),
      );

      if (!$output[$id]['status']) {
        drush_set_error("psm_instance_reload:{$output[$id]['service']}:{$output[$id]['name']}");
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_restart_validate($service_name = NULL, $instance_name = NULL) {
  drush_psm_instances_validate($service_name, $instance_name);
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_restart($service_name = NULL, $instance_name = NULL) {
  $group_name = drush_get_option('group', NULL);

  $instances = psm_get_instances($service_name, $instance_name, $group_name);
  $output = array();
  foreach ($instances as $instance_list) {
    foreach ($instance_list as $info) {
      $instance = psm_instance_factory($info);
      $id = $instance->id();
      $output[$id] = array(
        'service' => $instance->service(),
        'name' => $instance->name(),
        'status' => $instance->restart(),
      );

      if (!$output[$id]['status']) {
        drush_set_error("psm_instance_restart:{$output[$id]['service']}:{$output[$id]['name']}");
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_groups_validate($name = NULL) {
  if ($name && !array_key_exists($name, psm_info_group())) {
    drush_set_error("psm_group_unknown:$name");
  }
}

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

/**
 * Implements hook_psm_service_info().
 */
function psm_psm_service_info() {
  $info = array();

  $info['apache'] = array(
    'label' => 'Apache',
    'description' => dt('The Apache HTTP Server Project is a collaborative software development effort aimed at creating a robust, commercial-grade, featureful, and freely-available source code implementation of an HTTP (Web) server.'),
    'links' => array(
      'http://httpd.apache.org',
    ),
  );

  $info['jenkins'] = array(
    'label' => 'Jenkins',
    'description' => dt('An extendable open source continuous integration server.'),
    'links' => array(
      'http://jenkins-ci.org',
    ),
  );

  $info['memcache'] = array(
    'label' => 'MemCache',
    'description' => dt('Free & open source, high-performance, distributed memory object caching system.'),
    'links' => array(
      'http://memcached.org',
    ),
  );

  $info['mongodb'] = array(
    'label' => 'MongoDB',
    'description' => dt('MongoDB (from "humongous") is an open-source document database, and the leading NoSQL database. Written in C++.'),
    'links' => array(
      'http://www.mongodb.org',
    ),
  );

  $info['mysql'] = array(
    'label' => 'MySQL',
    'description' => dt("The world's most popular open source database."),
    'links' => array(
      'http://www.mysql.com',
      'http://www.percona.com',
      'https://mariadb.org',
    ),
  );

  $info['nginx'] = array(
    'label' => 'Nginx',
    'description' => dt('nginx [engine x] is an HTTP and reverse proxy server, as well as a mail proxy server, written by Igor Sysoev.'),
    'links' => array(
      'http://nginx.org',
    ),
  );

  $info['phpfpm'] = array(
    'label' => 'PhpFpm',
    'description' => dt('FPM (FastCGI Process Manager) is an alternative PHP FastCGI implementation with some additional features (mostly) useful for heavy-loaded sites.'),
    'links' => array(
      'http://www.php.net/manual/en/install.fpm.php',
    ),
  );

  $info['postgresql'] = array(
    'label' => 'PostgreSQL',
    'description' => dt('PostgreSQL is a powerful, open source object-relational database system.'),
    'links' => array(
      'http://www.postgresql.org',
    ),
  );

  $info['redis'] = array(
    'label' => 'redis',
    'description' => dt('Redis is an open source, BSD licensed, advanced key-value store.'),
    'links' => array(
      'http://redis.io',
    ),
  );

  $info['selenium'] = array(
    'label' => 'Selenium',
    'description' => dt('Primarily it is for automating web applications for testing purposes, but is certainly not limited to just that.'),
    'links' => array(
      'http://docs.seleniumhq.org',
    ),
  );

  $info['solr'] = array(
    'label' => 'Apache Solr',
    'description' => dt('SolrTM is the popular, blazing fast open source enterprise search platform from the Apache LuceneTM project.'),
    'links' => array(
      'http://lucene.apache.org/solr',
    ),
  );

  $info['Tomcat'] = array(
    'label' => 'Tomcat',
    'description' => dt('Apache Tomcat is an open source software implementation of the Java Servlet and JavaServer Pages technologies.'),
    'links' => array(
      'http://tomcat.apache.org',
    ),
  );

  $info['varnish'] = array(
    'label' => 'Varnish',
    'description' => dt('Varnish Cache is a web application accelerator also known as a caching HTTP reverse proxy.'),
    'links' => array(
      'https://www.varnish-cache.org',
    ),
  );

  $info['xapian'] = array(
    'label' => 'Xapian',
    'description' => dt("Xapian is an Open Source Search Engine Library, released under the GPL. It's written in C++."),
    'links' => array(
      'http://xapian.org',
    ),
  );

  return $info;
}

/**
 * Get service definitions.
 *
 * @param null|string $name
 *   Service identifier.
 *
 * @throws Exception
 *
 * @return array
 *   If the $name is omitted then all service.
 */
function psm_info_service($name = NULL) {
  static $services = NULL;

  if ($services === NULL) {
    // Collect the service definitions.
    $services = drush_command_invoke_all('psm_service_info');

    // Fill the default values.
    foreach (array_keys($services) as $service_name) {
      $service =& $services[$service_name];
      $service = _psm_info_service_defaults($service_name, $service);
    }

    // Allow to other modules to alter the service definitions.
    drush_command_invoke_all('psm_service_info_alter', $services);

    // Register the lib dir to autoloader.
    $command_files = drush_commandfile_list();
    $command_file_dir = dirname($command_files['psm']);

    try {
      $loader = require "$command_file_dir/vendor/autoload.php";
    }
    catch (Exception $e) {
      drush_set_error('psm_autoload_missing');

      return array();
    }

    foreach (array_keys($services) as $service_name) {
      $services[$service_name]['name'] = $service_name;

      if (!empty($service['lib_dir'])) {
        $prefixes = $loader->getPrefixes();
        if (!isset($prefixes['Drush']) || !in_array($service['lib_dir'], $prefixes['Drush'])) {
          $loader->add('Drush', $service['lib_dir']);
        }
      }
    }
  }

  if ($name) {
    if (!array_key_exists($name, $services)) {
      throw new Exception('Invalid argument', 1);
    }

    return $services[$name];
  }

  return $services;
}

/**
 * Get the instance definitions.
 *
 * @param string|null $name
 *   Instance identifier.
 *
 * @throws Exception
 *
 * @return array
 *   Instance definitions.
 */
function psm_info_instance($name = NULL) {
  static $instances = NULL;

  if ($instances === NULL) {
    $instances = _psm_config('instances');
    foreach (array_keys($instances) as $service_name) {
      foreach (array_keys($instances[$service_name]) as $instance_name) {
        $instances[$service_name][$instance_name]['name'] = $instance_name;
        $instances[$service_name][$instance_name]['service'] = $service_name;
      }
    }
  }

  if ($name) {
    if (!array_key_exists($name, $instances)) {
      throw new Exception('Invalid argument', 1);
    }

    return $instances[$name];
  }

  return $instances;
}

/**
 * Get all group definitions.
 *
 * @param string|null $name
 *   Group identifier.
 *
 * @throws Exception
 *
 * @return array
 *   Group definitions.
 */
function psm_info_group($name = NULL) {
  static $groups = NULL;

  if ($groups === NULL) {
    $groups = _psm_config('groups');
    foreach (array_keys($groups) as $group_name) {
      $groups[$group_name]['name'] = $group_name;
    }
  }

  if ($name) {
    if (!array_key_exists($name, $groups)) {
      throw new Exception('Invalid argument', 1);
    }

    return $groups[$name];
  }

  return $groups;
}

/**
 * Get a instance handler.
 *
 * @param array|string $info
 *   Instance definition or the service name.
 * @param string $name
 *   Instance name.
 *
 * @return \Drush\psm\InstanceInterface
 *   Instance handler.
 */
function psm_instance_factory($info, $name = NULL) {
  if (!is_array($info)) {
    $instances = psm_info_instance();
    $info = $instances[$info][$name];
  }

  // Make sure the autoloader is prepared.
  // @todo Better place to initialize the autoloader.
  $service = psm_info_service($info['service']);
  $class = (!empty($info['class']) ? $info['class'] : $service['class']);

  return $class::factory($info);
}

/**
 * Collect the instances by various filter.
 *
 * @param string|null $service_name
 *   Service identifier.
 * @param string|null $instance_name
 *   Instance identifier. Only used with the $service_name.
 * @param string|null $group_name
 *   Group identifier. Only used if the $service_name and the $instance_name are
 *   both omitted.
 *
 * @return array
 *   Multi dimension array with the instance definitions.
 *   service_name/instance_name/definition.
 */
function psm_get_instances($service_name = NULL, $instance_name = NULL, $group_name = NULL) {
  $instances = psm_info_instance();
  if (!$service_name && !$instance_name && $group_name) {
    $instances = psm_get_instances_by_group($group_name);
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

  return $instances;
}

/**
 * Filter instances by group name.
 *
 * @param string $group_name
 *   Group identifier.
 *
 * @return array
 *   Multi dimension array with the instance definitions.
 *   service_name/instance_name/definition.
 */
function psm_get_instances_by_group($group_name) {
  $instances_all = psm_info_instance();
  $instances = array();
  $groups = psm_info_group();
  foreach ($groups[$group_name]['instances'] as $group_service_name => $group_instances) {
    $instances[$group_service_name] = array_intersect_key(
      $instances_all[$group_service_name],
      array_flip($group_instances)
    );
  }

  return $instances;
}

/**
 * Fill the default values of a service definition.
 *
 * @param string $service_name
 *   Service identifier.
 * @param array $info
 *   Base definition.
 *
 * @return array
 *   Fully populated service definition.
 */
function _psm_info_service_defaults($service_name, array $info = array()) {
  $info += array(
    // Provider command file.
    // Required.
    'command_file' => 'psm',
    // Machine name of the service.
    // Required, but force populated later.
    'name' => $service_name,
    // Human name of the service
    // Required.
    'label' => $service_name,
    // Long description.
    // Optional.
    'description' => NULL,

    'lib_dir' => NULL,
    'class' => '',
  );

  if (empty($info['class'])) {
    $info['class'] = '\\' . implode('\\', array(
      'Drush',
      $info['command_file'],
      'Plugin',
      'psm',
      'Instance',
      _psm_camel_case($service_name),
    ));
  }

  if (empty($info['lib_dir']) && $info['command_file']) {
    $command_files = drush_commandfile_list();
    $command_file_dir = dirname($command_files[$info['command_file']]);

    if (is_dir("$command_file_dir/lib")) {
      $info['lib_dir'] = "$command_file_dir/lib";
    }
    elseif (strrpos($command_file_dir, '/includes') && is_dir("$command_file_dir/../lib")) {
      $info['lib_dir'] = "$command_file_dir/../lib";
    }
  }

  return $info;
}

/**
 * Transform a snake case machine name to camel case.
 *
 * @param string $name
 *   String in snake case.
 *
 * @return string
 *   Camel case string.
 */
function _psm_camel_case($name) {
  $camel_case = function ($matches) {
    return strtoupper(trim($matches[0], '_'));
  };

  return preg_replace_callback('/(^|_)./', $camel_case, $name);
}

/**
 * Array merge through different drushrc files.
 *
 * @param string $subject
 *   Base variable name to merge.
 *
 * @return array
 *   Array structure is depend on the
 */
function _psm_config($subject) {
  $return = array();

  foreach (_psm_config_files($subject) as $file) {
    $$subject = NULL;
    $include_result = @include_once $file;
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
 * Collect the file names.
 *
 * @param string $subject
 *   File name prefix.
 * @param string[]|null $paths
 *   Directories where search the drushrc files in.
 *
 * @return string[]
 *   File names.
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
 * Collect the directories where a drushrc file can be.
 *
 * @param string $subject
 *   File name prefix.
 *
 * @return string[]
 *   Array of directory names.
 */
function _psm_config_paths($subject) {
  return (array) drush_get_context('ALIAS_PATH', array());
}
