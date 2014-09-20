<?php

/**
 * @file
 * Personal Service Manager.
 */

/**
 * Implements hook_drush_command().
 */
function psm_drush_command() {
  $base = array(
    'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    'required-arguments' => FALSE,
    'arguments' => array(
      'instance_list' => dt('Name of the instances.'),
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

  return array(
    'docs-psm' => array(
      'description' => dt('Personal service manager.'),
      'hidden' => TRUE,
      'topic' => TRUE,
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
      'callback' => 'drush_print_file',
      'callback arguments' => array(__DIR__ . '/README.md'),
    ),

    'psm-services' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
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
    ),

    'psm-instances' => array(
      'aliases' => array('psm'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
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
        'drush psm-instances memcache.3306' => dt('Description of what this command will do.'),
      ),
      'engines' => array(
        'outputformat' => array(
          'default' => 'table',
          'pipe-format' => 'list',
          'field-labels' => array(
            'service_label' => dt('Service'),
            'service_name' => dt('Service ID'),
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
    ),

    'psm-groups' => array(
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
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
    ),

    'psm-status' => array(
      'description' => dt('Something helpful description.'),
      'examples' => array(
        'drush psm-status' => dt('Status messages about the all available services and instances.'),
        'drush psm-status memcache' => dt('Status messages about the all instances of the memcache service.'),
        'drush psm-status memcache.11211' => dt('Status messages of the memcache service instance where the identifier is 11211.'),
        'drush psm-status memcache.11211,11212' => dt('Status messages of the memcache service instance where the identifier is 11211 or 11212.'),
      ),
    ) + $base,

    'psm-start' => array(
      'description' => dt('Something helpful description.'),
      'examples' => array(
        'drush psm-start' => dt('Start the all available instances of all available services.'),
        'drush psm-start memcache' => dt('Start all configured Memcache instances.'),
        'drush psm-start memcache.11211' => dt('Start Memcache instance which is identified with 11211.'),
        'drush psm-start memcache.11212:mysql.3306' => dt('Start the "11211" instance of Memcache service and the "3306" instance of MySQL service.'),
      ),
    ) + $base,

    'psm-stop' => array(
      'description' => dt('Something helpful description.'),
      'examples' => array(
        'drush psm-stop' => dt('Stop the all available instances of all available services.'),
        'drush psm-stop memcache' => dt('Stop all configured Memcache instances.'),
        'drush psm-stop memcache.11211' => dt('Start Memcache instance which is identified with 11211.'),
        'drush psm-stop memcache.11212:mysql.3306' => dt('Stop the "11211" instance of Memcache service and the "3306" instance of MySQL service.'),
      ),
    ) + $base,

    'psm-restart' => array(
      'description' => dt('Something helpful description.'),
      'examples' => array(
        'drush psm-restart' => dt('Restart the all available instances of all available services.'),
        'drush psm-restart memcache' => dt('@todo'),
        'drush psm-restart memcache.11211' => dt('Restart Memcache instance which is identified with 11211.'),
        'drush psm-restart memcache.11212:mysql.3306' => dt('Restart the "11211" instance of Memcache service and the "3306" instance of MySQL service.'),
      ),
    ) + $base,

    'psm-reload' => array(
      'description' => dt('Something helpful description.'),
      'examples' => array(
        'drush psm-reload' => dt('Restart the all available instances of all available services.'),
        'drush psm-reload memcache' => dt('@todo'),
        'drush psm-reload memcache.11211' => dt('@todo.'),
      ),
    ) + $base,
  );
}

/**
 * Implements hook_drush_help().
 */
function psm_drush_help($subject = NULL) {
  $subject_args = explode(':', $subject);
  $subject = array_shift($subject_args);
  if ($subject_args && in_array($subject, _psm_drush_log_entry_types())) {
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
      $help[] = dt('Service "@name" is not exists.', array('@name' => implode(':', $subject_args)));
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
      $help = array_merge($help, (array) drush_shell_exec_output());

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
 * Implements drush_COMMAND_init().
 */
function drush_psm_services_init() {
  psm_autoloader_add_lib_dirs();
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
 * Implements drush_COMMAND_init().
 */
function drush_psm_instances_init() {
  psm_autoloader_add_lib_dirs();
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_instances_validate() {
  _psm_validate_instance_filter_parameters(
    implode(':', func_get_args()),
    drush_get_option('group')
  );
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_instances() {
  $instance_list = implode(':', func_get_args());
  $instance_list = _psm_parse_instance_list($instance_list);

  $instances = psm_get_instances($instance_list, drush_get_option('group'));
  $services = psm_info_service();

  $output = array();
  foreach ($instances as $service_name => $instance_list) {
    $service = $services[$service_name];
    foreach ($instance_list as $name => $info) {
      $instance = psm_instance_factory($info);
      $output[$name] = array(
        'service_name' => $service_name,
        'service_label' => $service['label'],
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
 * Implements drush_COMMAND_init().
 */
function drush_psm_status_init() {
  psm_autoloader_add_lib_dirs();
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_status_validate() {
  _psm_validate_instance_filter_parameters(
    implode(':', func_get_args()),
    drush_get_option('group')
  );
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_status() {
  $instance_list = implode(':', func_get_args());
  $instance_list = _psm_parse_instance_list($instance_list);

  $instances = psm_get_instances($instance_list, drush_get_option('group'));
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
 * Implements drush_COMMAND_init().
 */
function drush_psm_start_init() {
  psm_autoloader_add_lib_dirs();
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_start_validate() {
  _psm_validate_instance_filter_parameters(
    implode(':', func_get_args()),
    drush_get_option('group')
  );
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_start() {
  $instance_list = implode(':', func_get_args());
  $instance_list = _psm_parse_instance_list($instance_list);

  $instances = psm_get_instances($instance_list, drush_get_option('group'));
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
      else {
        $message = dt('Instance @service:@name started.', array(
            '@service' => $output[$id]['service'],
            '@name' => $output[$id]['name'],
          ));
        drush_log($message, 'completed');
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_init().
 */
function drush_psm_stop_init() {
  psm_autoloader_add_lib_dirs();
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_stop_validate() {
  _psm_validate_instance_filter_parameters(
    implode(':', func_get_args()),
    drush_get_option('group')
  );
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_stop() {
  $instance_list = implode(':', func_get_args());
  $instance_list = _psm_parse_instance_list($instance_list);

  $instances = psm_get_instances($instance_list, drush_get_option('group'));
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
      else {
        $message = dt('Instance @service:@name stopped.', array(
            '@service' => $output[$id]['service'],
            '@name' => $output[$id]['name'],
          ));
        drush_log($message, 'completed');
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_init().
 */
function drush_psm_reload_init() {
  psm_autoloader_add_lib_dirs();
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_reload_validate() {
  _psm_validate_instance_filter_parameters(
    implode(':', func_get_args()),
    drush_get_option('group')
  );
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_reload() {
  $instance_list = implode(':', func_get_args());
  $instance_list = _psm_parse_instance_list($instance_list);

  $instances = psm_get_instances($instance_list, drush_get_option('group'));
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
      else {
        $message = dt('Instance @service:@name reloaded.', array(
            '@service' => $output[$id]['service'],
            '@name' => $output[$id]['name'],
          ));
        drush_log($message, 'completed');
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_init().
 */
function drush_psm_restart_init() {
  psm_autoloader_add_lib_dirs();
}

/**
 * Implements drush_COMMAND_validate().
 */
function drush_psm_restart_validate() {
  _psm_validate_instance_filter_parameters(
    implode(':', func_get_args()),
    drush_get_option('group')
  );
}

/**
 * Implements drush_COMMAND().
 */
function drush_psm_restart() {
  $instance_list = implode(':', func_get_args());
  $instance_list = _psm_parse_instance_list($instance_list);

  $instances = psm_get_instances($instance_list, drush_get_option('group'));
  $output = array();
  foreach ($instances as $instance_list) {
    foreach ($instance_list as $info) {
      $instance = psm_instance_factory($info);
      if (!$instance->status()) {
        continue;
      }

      $id = $instance->id();
      $output[$id] = array(
        'service' => $instance->service(),
        'name' => $instance->name(),
        'status' => $instance->restart(),
      );

      if (!$output[$id]['status']) {
        drush_set_error("psm_instance_restart:{$output[$id]['service']}:{$output[$id]['name']}");
      }
      else {
        $message = dt('Instance @service:@name restarted.', array(
            '@service' => $output[$id]['service'],
            '@name' => $output[$id]['name'],
          ));
        drush_log($message, 'completed');
      }
    }
  }

  return $output;
}

/**
 * Implements drush_COMMAND_init().
 */
function drush_psm_groups_init() {
  psm_autoloader_add_lib_dirs();
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
  return array(
    'apache' => array(
      'label' => 'Apache',
      'description' => dt('The Apache HTTP Server Project is a collaborative software development effort aimed at creating a robust, commercial-grade, featureful, and freely-available source code implementation of an HTTP (Web) server.'),
      'links' => array(
        'home' => 'http://httpd.apache.org',
      ),
    ),

    'elasticsearch' => array(
      'label' => 'Elasticsearch',
      'description' => dt('Elasticsearch is a flexible and powerful open source, distributed, real-time search and analytics engine.'),
      'links' => array(
        'home' => 'http://www.elasticsearch.org/',
      ),
    ),

    'jenkins' => array(
      'label' => 'Jenkins',
      'description' => dt('An extendable open source continuous integration server.'),
      'links' => array(
        'home' => 'http://jenkins-ci.org',
      ),
    ),

    'memcache' => array(
      'label' => 'MemCache',
      'description' => dt('Free & open source, high-performance, distributed memory object caching system.'),
      'links' => array(
        'home' => 'http://memcached.org',
      ),
    ),

    'mongodb' => array(
      'label' => 'MongoDB',
      'description' => dt('MongoDB (from "humongous") is an open-source document database, and the leading NoSQL database. Written in C++.'),
      'links' => array(
        'home' => 'http://www.mongodb.org',
      ),
    ),

    'mysql' => array(
      'label' => 'MySQL',
      'description' => dt("The world's most popular open source database."),
      'links' => array(
        'home' => 'http://www.mysql.com',
        'percona' => 'http://www.percona.com',
        'mariadb' => 'https://mariadb.org',
      ),
    ),

    'nginx' => array(
      'label' => 'Nginx',
      'description' => dt('nginx [engine x] is an HTTP and reverse proxy server, as well as a mail proxy server, written by Igor Sysoev.'),
      'links' => array(
        'home' => 'http://nginx.org',
      ),
    ),

    'phpfpm' => array(
      'label' => 'PhpFpm',
      'description' => dt('FPM (FastCGI Process Manager) is an alternative PHP FastCGI implementation with some additional features (mostly) useful for heavy-loaded sites.'),
      'links' => array(
        'home' => 'http://www.php.net/manual/en/install.fpm.php',
      ),
    ),

    'postgresql' => array(
      'label' => 'PostgreSQL',
      'description' => dt('PostgreSQL is a powerful, open source object-relational database system.'),
      'links' => array(
        'home' => 'http://www.postgresql.org',
      ),
    ),

    'redis' => array(
      'label' => 'redis',
      'description' => dt('Redis is an open source, BSD licensed, advanced key-value store.'),
      'links' => array(
        'home' => 'http://redis.io',
      ),
    ),

    'selenium' => array(
      'label' => 'Selenium',
      'description' => dt('Primarily it is for automating web applications for testing purposes, but is certainly not limited to just that.'),
      'links' => array(
        'home' => 'http://docs.seleniumhq.org',
      ),
    ),

    'solr' => array(
      'label' => 'Apache Solr',
      'description' => dt('SolrTM is the popular, blazing fast open source enterprise search platform from the Apache LuceneTM project.'),
      'links' => array(
        'home' => 'http://lucene.apache.org/solr',
      ),
      'class' => array(
        1 => '\Drush\psm\Plugin\psm\Instance\Solr1',
        3 => '\Drush\psm\Plugin\psm\Instance\Solr3',
        4 => '\Drush\psm\Plugin\psm\Instance\Solr4',
      ),
    ),

    'tomcat' => array(
      'label' => 'Tomcat',
      'description' => dt('Apache Tomcat is an open source software implementation of the Java Servlet and JavaServer Pages technologies.'),
      'links' => array(
        'home' => 'http://tomcat.apache.org',
      ),
    ),

    'varnish' => array(
      'label' => 'Varnish',
      'description' => dt('Varnish Cache is a web application accelerator also known as a caching HTTP reverse proxy.'),
      'links' => array(
        'home' => 'https://www.varnish-cache.org',
      ),
    ),

    'xapian' => array(
      'label' => 'Xapian',
      'description' => dt("Xapian is an Open Source Search Engine Library, released under the GPL. It's written in C++."),
      'links' => array(
        'home' => 'http://xapian.org',
      ),
    ),
  );
}

/**
 * Implements hook_psm_instance_info().
 */
function psm_psm_instance_info() {
  return _psm_config('instances');
}

/**
 * Implements hook_psm_group_info().
 */
function psm_psm_group_info() {
  return _psm_config('groups');
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

  $cache_id = drush_get_cid(__FUNCTION__);
  if ($services === NULL) {
    $cache = drush_cache_get($cache_id);
    if ($cache && !empty($cache->data)) {
      $services = $cache->data;
    }
  }

  if ($services === NULL) {
    $services = array();
    // Collect the service definitions.
    foreach (drush_command_implements('psm_service_info') as $command_file) {
      $function = "{$command_file}_psm_service_info";
      $result = $function();
      if (is_array($result)) {
        // Fill the default values.
        foreach (array_keys($result) as $service_name) {
          if (!_psm_valid_machine_name($service_name)) {
            throw new Exception('Invalid service name.');
          }

          _psm_info_service_defaults($command_file, $service_name, $result[$service_name]);
        }
        $services += $result;
      }
    }

    // Allow to other modules to alter the service definitions.
    drush_command_invoke_all_ref('psm_service_info_alter', $services);

    drush_cache_set($cache_id, $services);
  }

  if ($name) {
    if (!isset($services[$name])) {
      throw new Exception('Invalid argument', 1);
    }

    return $services[$name];
  }

  return $services;
}

/**
 * Get the instance definitions.
 *
 * @param string|null $service
 *   Service identifier.
 * @param string|null $name
 *   Instance identifier.
 *
 * @throws Exception
 * @return array
 *   Instance definitions.
 */
function psm_info_instance($service = NULL, $name = NULL) {
  static $instances_all = NULL;

  $cache_id = drush_get_cid(__FUNCTION__);
  if ($instances_all === NULL) {
    $cache = drush_cache_get($cache_id);
    if ($cache && !empty($cache->data)) {
      $instances_all = $cache->data;
    }
  }

  if ($instances_all === NULL) {
    // Collect the service definitions.
    $instances_all = drush_command_invoke_all('psm_instance_info');

    // Fill the default values.
    foreach (array_keys($instances_all) as $service_name) {
      foreach (array_keys($instances_all[$service_name]) as $instance_name) {
        if (!_psm_valid_machine_name($instance_name)) {
          throw new Exception('Invalid instance name.');
        }

        _psm_info_instance_defaults($service_name, $instance_name, $instances_all[$service_name][$instance_name]);
      }
    }

    // Allow to other modules to alter the instance definitions.
    drush_command_invoke_all_ref('psm_instance_info_alter', $instances_all);

    foreach (array_keys($instances_all) as $service_name) {
      foreach (array_keys($instances_all[$service_name]) as $instance_name) {
        $class = psm_instance_get_class($instances_all[$service_name][$instance_name]);
        $instances_all[$service_name][$instance_name] += $class::defaultInfo($instances_all[$service_name][$instance_name]);
      }
    }

    drush_cache_set($cache_id, $instances_all);
  }

  if ($service) {
    if (!isset($instances_all[$service])) {
      // @todo Error message.
      throw new Exception('Invalid argument', 1);
    }

    if ($name) {
      if (!isset($instances_all[$service][$name])) {
        // @todo Error message.
        throw new Exception('Invalid argument', 1);
      }

      return $instances_all[$service][$name];
    }

    return $instances_all[$service];
  }

  return $instances_all;
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

  $cache_id = drush_get_cid(__FUNCTION__);
  if ($groups === NULL) {
    $cache = drush_cache_get($cache_id);
    if ($cache && !empty($cache->data)) {
      $groups = $cache->data;
    }
  }

  if ($groups === NULL) {
    // Collect the group definitions.
    $groups = drush_command_invoke_all('psm_group_info');
    foreach (array_keys($groups) as $group_name) {
      if (!_psm_valid_machine_name($group_name)) {
        throw new Exception('Invalid group name.');
      }

      $groups[$group_name]['name'] = $group_name;
    }

    // Allow to other modules to alter the group definitions.
    drush_command_invoke_all('psm_group_info_alter', $groups);

    drush_cache_set($cache_id, $groups);
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
 * Add all lib directory to the autoloader.
 */
function psm_autoloader_add_lib_dirs() {
  foreach (drush_commandfile_list() as $file_path) {
    $dir = dirname($file_path);
    if (is_dir("$dir/lib")) {
      psm_autoloader_add_lib_dir("$dir/lib");
    }
    elseif (preg_match('@.+/includes$@', $dir) && is_dir("$dir/../lib")) {
      psm_autoloader_add_lib_dir("$dir/../lib");
    }
  }
}

/**
 * Add a root directory to the PSR-0 autoloader.
 *
 * @param string $lib_dir
 *   Directory path.
 *
 * @return bool
 *   Directory is added or not.
 */
function psm_autoloader_add_lib_dir($lib_dir) {
  static $loader = NULL;

  if ($loader === NULL) {
    // Register the lib dir to autoloader.
    $command_files = drush_commandfile_list();
    $psm_dir = dirname($command_files['psm']);
    try {
      $loader = require "$psm_dir/vendor/autoload.php";
    }
    catch (Exception $e) {
      $loader = FALSE;
      drush_set_error('psm_autoload_missing');
    }
  }

  if ($loader) {
    $prefixes = $loader->getPrefixes();
    if (!isset($prefixes['Drush']) || !in_array($lib_dir, $prefixes['Drush'])) {
      $loader->add('Drush', $lib_dir);
    }

    return TRUE;
  }

  return FALSE;
}

/**
 * Get a instance handler.
 *
 * @param array|string $info
 *   Instance definition or the service name.
 * @param string $name
 *   Instance name.
 *
 * @throws Exception
 *
 * @return \Drush\psm\InstanceInterface
 *   Instance handler.
 */
function psm_instance_factory($info, $name = NULL) {
  if (!is_array($info)) {
    $instances = psm_info_instance();
    $info = $instances[$info][$name];
  }

  $class = psm_instance_get_class($info);

  return $class::factory($info);
}

/**
 * Get instance handler class name.
 *
 * @param array $info
 *   Instance definition.
 *
 * @return string
 *   Name of the instance handler class.
 *
 * @throws Exception
 */
function psm_instance_get_class(array $info) {
  $service = psm_info_service($info['service']);
  if (!empty($info['class'])) {
    $class = $info['class'];
  }
  elseif (!empty($info['api_version'])) {
    if (is_array($service['class'])) {
      if (empty($service['class'][$info['api_version']])) {
        throw new Exception('Instance handler is not exists for API version: ' . $info['api_version']);
      }

      $class = $service['class'][$info['api_version']];
    }
    else {
      $class = $service['class'];
    }
  }
  elseif (!empty($service['class'])) {
    $class = $service['class'];
  }
  else {
    throw new Exception('Instance handler is not defined.');
  }

  return $class;
}

/**
 * Collect the instances by various filter.
 *
 * @param array|null $instance_list
 *   Array of instance identifiers.
 * @param string|null $group_name
 *   Group identifier. Only used if the $service_name and the $instance_name are
 *   both omitted.
 *
 * @return array
 *   Multi dimension array with the instance definitions.
 *   service_name/instance_name/definition.
 */
function psm_get_instances($instance_list = NULL, $group_name = NULL) {
  if (!$instance_list && $group_name) {
    return psm_get_instances_by_group($group_name);
  }

  $instances = psm_info_instance();
  if ($instance_list) {
    $return = array();
    foreach ($instance_list as $service_name => $list) {
      $return[$service_name] = (!$list ?
        $instances[$service_name] : array_intersect_key($instances[$service_name], array_flip($list))
      );
    }

    return $return;
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
 * Log entry prefixes.
 *
 * @return string[]
 *   Array of log entry prefixes.
 */
function _psm_drush_log_entry_types() {
  return array(
    'warning',
    'cancel',
    'failed',
    'error',
    'ok',
    'completed',
    'success',
    'status',
    'notice',
    'message',
    'info',
  );
}

/**
 * Fill the default values of a service definition.
 *
 * @param string $command_file
 *   Name of the command file.
 * @param string $service_name
 *   Service identifier.
 * @param array $info
 *   Base definition.
 *
 * @return array
 *   Fully populated service definition.
 */
function _psm_info_service_defaults($command_file, $service_name, array &$info = array()) {
  $info['command_file'] = $command_file;
  $info['name'] = $service_name;

  $info += array(
    // Human name of the service
    // Required.
    'label' => $service_name,
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
}

/**
 * Fill the default values of a service definition.
 *
 * @param string $service_name
 *   Service identifier.
 * @param string $instance_name
 *   Instance identifier.
 * @param array $info
 *   Base definition.
 *
 * @return array
 *   Fully populated instance definition.
 */
function _psm_info_instance_defaults($service_name, $instance_name, array &$info = array()) {
  $info['service'] = $service_name;
  $info['name'] = $instance_name;
  $info += array(
    'label' => $instance_name,
  );
}

/**
 * Helper function to validate the instance filter command line arguments.
 *
 * @param string $instance_list
 *   service_name1.instance_name1,instance_name2
 * @param string $group_name
 *   Group identifier.
 */
function _psm_validate_instance_filter_parameters($instance_list, $group_name) {
  $instance_list = _psm_parse_instance_list($instance_list);
  _psm_validate_instance_list($instance_list);

  if ($group_name) {
    if ($instance_list) {
      drush_log(dt('The group option is ignored because the service name is specified'), 'warning');
    }

    $groups = psm_info_group();
    if (!array_key_exists($group_name, $groups)) {
      drush_set_error("psm_group_unknown:$group_name");
    }
    else {
      _psm_validate_instance_list($groups[$group_name]['instances']);
    }
  }
}

/**
 * Parse the command line argument.
 *
 * @param string $list
 *   service_name1:instance_name1,instance_name2
 *
 * @return array
 *   Instance list.
 */
function _psm_parse_instance_list($list) {
  if (!trim($list)) {
    return array();
  }

  $return = array();
  $instances = psm_info_instance();
  $service_list = explode(':', $list);
  foreach ($service_list as $service) {
    $service_name = $service;
    $instance_list = array();

    $service = trim($service, ':,.');
    if (strpos($service, '.')) {
      list($service_name, $instance_list) = explode('.', $service, 2);
      $instance_list = explode(',', $instance_list);
    }

    if (!$instance_list && isset($instances[$service_name])) {
      // All instances.
      $instance_list = array_keys($instances[$service_name]);
    }

    if (!isset($return[$service_name])) {
      $return[$service_name] = array();
    }

    $return[$service_name] = array_merge($return[$service_name], $instance_list);
  }

  foreach (array_keys($return) as $service_name) {
    $return[$service_name] = array_unique($return[$service_name]);
  }

  return $return;
}

/**
 * Instance list validator.
 *
 * @param array $instance_list
 *   Instance list.
 */
function _psm_validate_instance_list($instance_list) {
  $instances = psm_info_instance();

  foreach ($instance_list as $service_name => $list) {
    if (!isset($instances[$service_name])) {
      drush_set_error("psm_service_unknown:$service_name");

      continue;
    }

    $instance_names = array_diff($list, array_keys($instances[$service_name]));
    foreach ($instance_names as $instance_name) {
      drush_set_error("psm_instance_unknown:$service_name:$instance_name");
    }
  }
}

/**
 * Check the $name is a valid machine name.
 *
 * @param string $name
 *   Machine name suggestion.
 *
 * @return bool
 *   TRUE if the $name is a valid machine name or FALSE otherwise.
 */
function _psm_valid_machine_name($name) {
  return preg_match('/^[a-z0-9][a-z0-9_-]*$/', $name) && !preg_match('/(_|-){2,}/', $name);
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
    $paths = _psm_config_paths();
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
 * @return string[]
 *   Array of directory names.
 */
function _psm_config_paths() {
  return (array) drush_get_context('ALIAS_PATH', array());
}

/**
 * Merges multiple arrays, recursively, and returns the merged array.
 */
function _psm_array_merge_deep(array $arrays, $preserve_integer_keys = FALSE) {
  $result = array();
  foreach ($arrays as $array) {
    foreach ($array as $key => $value) {
      // Renumber integer keys as array_merge_recursive() does unless
      // $preserve_integer_keys is set to TRUE. Note that PHP automatically
      // converts array keys that are integer strings (e.g., '1') to integers.
      if (is_int($key) && !$preserve_integer_keys) {
        $result[] = $value;
      }
      // Recurse when both values are arrays.
      elseif (isset($result[$key]) && is_array($result[$key]) && is_array($value)) {
        $result[$key] = _psm_array_merge_deep(array($result[$key], $value), $preserve_integer_keys);
      }
      // Otherwise, use the latter value, overriding any previous value.
      else {
        $result[$key] = $value;
      }
    }
  }

  return $result;
}
