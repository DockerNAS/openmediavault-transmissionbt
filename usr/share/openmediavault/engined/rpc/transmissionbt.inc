<?php

/**
 * Copyright (C) 2009-2012 Volker Theile <volker.theile@openmediavault.org>
 * Copyright (C) 2011-2012 Marcel Beck <marcel.beck@mbeck.org>
 * Copyright (C) 2013-2015 OpenMediaVault Plugin Developers.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
require_once 'openmediavault/config.inc';
require_once 'openmediavault/error.inc';
require_once 'openmediavault/notify.inc';
require_once 'openmediavault/rpcservice.inc';

class OMVRpcServiceTransmissionBt extends OMVRpcServiceAbstract
{
    /**
     * The main event path.
     *
     * @var string
     */
    private $eventMessagePath = 'org.openmediavault.services.transmissionbt';

    /**
     * Get the main configuration XPath.
     *
     * @return string
     */
    private function getXpath()
    {
        return '/config/services/transmissionbt';
    }

    /**
     * Get the bandwidth XPath.
     *
     * @return string
     */
    public function getBandwidthXpath()
    {
        return sprintf('%s/modules/bandwidth', $this->getXpath());
    }

    /**
     * Get the blocklists XPath.
     *
     * @return string
     */
    public function getBlocklistsXpath()
    {
        return sprintf('%s/modules/blocklists', $this->getXpath());
    }

    /**
    * Get the files and locations XPath.
    *
    * @return string
    */
    public function getFilesAndLocationsXpath()
    {
        return sprintf('%s/modules/filesandlocations', $this->getXpath());
    }

    /**
     * Get the peer XPath.
     *
     * @return string
     */
    public function getPeerXpath()
    {
        return sprintf('%s/modules/peer', $this->getXpath());
    }

    /**
     * Get the queuing XPath.
     *
     * @return string
     */
    public function getQueuingXpath()
    {
        return sprintf('%s/modules/queuing', $this->getXpath());
    }

    /**
     * Get the RPC XPath.
     *
     * @return string
     */
    public function getRpcXpath()
    {
        return sprintf('%s/modules/rpc', $this->getXpath());
    }

    /**
     * Get the scheduling XPath.
     *
     * @return string
     */
    public function getSchedulingXpath()
    {
        return sprintf('%s/modules/scheduling', $this->getXpath());
    }

    /**
     * Get the RPC service name.
     *
     * @return string
     */
    public function getName()
    {
        return 'TransmissionBt';
    }

    /**
     * Initialize the RPC service.
     *
     * @return void
     */
    public function initialize()
    {
        $this->registerMethod('getSettings');
        $this->registerMethod('setSettings');
        $this->registerMethod('getBandwidthSettings');
        $this->registerMethod('setBandwidthSettings');
        $this->registerMethod('getBlocklistsSettings');
        $this->registerMethod('setBlocklistsSettings');
        $this->registerMethod('getFilesAndLocationsSettings');
        $this->registerMethod('setFilesAndLocationsSettings');
        $this->registerMethod('getPeerSettings');
        $this->registerMethod('setPeerSettings');
        $this->registerMethod('getQueuingSettings');
        $this->registerMethod('setQueuingSettings');
        $this->registerMethod('getRpcSettings');
        $this->registerMethod('setRpcSettings');
        $this->registerMethod('getSchedulingSettings');
        $this->registerMethod('setSchedulingSettings');
    }

    /**
     * Get the general settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getXpath());
        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXpath()
            );
        }

        // Modify the result object.
        $object['enable'] = boolval($object['enable']);
        $object['cache-size-mb'] = intval($object['cache-size-mb']);
        $object['dht-enabled'] = boolval($object['dht-enabled']);
        $object['encryption'] = intval($object['encryption']);
        $object['lazy-bitfield-enabled'] = boolval($object['lazy-bitfield-enabled']);
        $object['lpd-enabled'] = boolval($object['lpd-enabled']);
        $object['message-level'] = intval($object['message-level']);
        $object['pex-enabled'] = boolval($object['pex-enabled']);
        $object['scrape-paused-torrents-enabled'] = boolval($object['scrape-paused-torrents-enabled']);
        $object['script-torrent-done-enabled'] = boolval($object['script-torrent-done-enabled']);
        $object['utp-enabled'] = boolval($object['utp-enabled']);

        unset($object['modules']);

        return $object;
    }

    /**
     * Set the general settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "enable": {
                        "type": "boolean"
                    },
                    "cache-size-mb": {
                        "type": "integer"
                    },
                    "dht-enabled": {
                        "type": "boolean"
                    },
                    "encryption": {
                        "type": "integer",
                        "enum": [0, 1, 2]
                    },
                    "lazy-bitfield-enabled": {
                        "type": "boolean"
                    },
                    "lpd-enabled": {
                        "type": "boolean"
                    },
                    "message-level": {
                        "type": "integer",
                        "enum": [0, 1, 2, 3]
                    },
                    "pex-enabled": {
                        "type": "boolean"
                    },
                    "scrape-paused-torrents-enabled": {
                        "type": "boolean"
                    },
                    "script-torrent-done-enabled": {
                        "type": "boolean"
                    },
                    "script-torrent-done-filename": {
                        "type": "string"
                    },
                    "utp-enabled": {
                        "type": "boolean"
                    }
                }
            }'
        );

        // Get the existing configuration object.
        $oldObject = $xmlConfig->get($this->getXpath());

        if (is_null($oldObject)) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED, $this->getXpath());
        }

        // If no directory for downloads is set and the user is trying to enable
        // the service, abort the save.
        $downloadFolderRef = $oldObject['modules']
                                       ['filesandlocations']
                                       ['download.sharedfolderref'];
        $msg = 'No directory for downloads set, please set one before enabling the service';

        if ($params['enable'] === true && empty($downloadFolderRef)) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SAVE_FAILED, $msg);
        }

        // Prepare the configuration data.
        $object = [
            'enable' => boolval($params['enable']),
            'cache-size-mb' => $params['cache-size-mb'],
            'dht-enabled' => boolval($params['dht-enabled']),
            'encryption' => $params['encryption'],
            'lazy-bitfield-enabled' => boolval($params['lazy-bitfield-enabled']),
            'lpd-enabled' => boolval($params['lpd-enabled']),
            'message-level' => $params['message-level'],
            'pex-enabled' => boolval($params['pex-enabled']),
            'scrape-paused-torrents-enabled' => boolval($params['scrape-paused-torrents-enabled']),
            'script-torrent-done-enabled' => boolval($params['script-torrent-done-enabled']),
            'script-torrent-done-filename' => $params['script-torrent-done-filename'],
            'utp-enabled' => boolval($params['utp-enabled']),
            'modules' => isset($oldObject['modules']) ? $oldObject['modules'] : [],
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get bandwidth settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getBandwidthSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getBandwidthXpath());
        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getBandwidthXpath()
            );
        }

        // Modify the result data.
        $object['speed-limit-down'] = intval($object['speed-limit-down']);
        $object['speed-limit-down-enabled'] = boolval($object['speed-limit-down-enabled']);
        $object['speed-limit-up'] = intval($object['speed-limit-up']);
        $object['speed-limit-up-enabled'] = boolval($object['speed-limit-up-enabled']);
        $object['upload-slots-per-torrent'] = intval($object['upload-slots-per-torrent']);
        $object['alt-speed-enabled'] = boolval($object['alt-speed-enabled']);
        $object['alt-speed-up'] = intval($object['alt-speed-up']);
        $object['alt-speed-down'] = intval($object['alt-speed-down']);

        return $object;
    }

    /**
     * Set bandwidth settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setBandwidthSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "alt-speed-enabled": {
                        "type": "boolean"
                    },
                    "alt-speed-up": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "alt-speed-down": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "speed-limit-down": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "speed-limit-down-enabled": {
                        "type": "boolean"
                    },
                    "speed-limit-up": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "speed-limit-up-enabled": {
                        "type": "boolean"
                    },
                    "upload-slots-per-torrent": {
                        "type": "integer"
                    }
                }
            }'
        );

        $object = [
            'alt-speed-enabled' => boolval($params['alt-speed-enabled']),
            'alt-speed-up' => $params['alt-speed-up'],
            'alt-speed-down' => $params['alt-speed-down'],
            'speed-limit-down' => $params['speed-limit-down'],
            'speed-limit-down-enabled' => boolval($params['speed-limit-down-enabled']),
            'speed-limit-up' => $params['speed-limit-up'],
            'speed-limit-up-enabled' => boolval($params['speed-limit-up-enabled']),
            'upload-slots-per-torrent' => $params['upload-slots-per-torrent'],
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getBandwidthXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get blocklists settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getBlocklistsSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getBlocklistsXpath());
        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getBlocklistsXpath()
            );
        }

        // Modify the result object.
        $object['blocklist-enabled'] = boolval($object['blocklist-enabled']);
        $object['blocklist-sync-enabled'] = boolval($object['blocklist-sync-enabled']);

        return $object;
    }

    /**
     * Set blocklists settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setBlocklistsSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "blocklist-enabled": {
                        "type": "boolean"
                    },
                    "blocklist-url": {
                        "type": "string"
                    },
                    "blocklist-sync-frequency": {
                        "type": "string",
                        "enum": [
                            "hourly",
                            "daily",
                            "weekly",
                            "monthly"
                        ]
                    },
                    "blocklist-sync-enabled": {
                        "type": "boolean"
                    }
                }
            }'
        );

        // Prepare the configuration data.
        $object = [
            'blocklist-enabled' => boolval($params['blocklist-enabled']),
            'blocklist-sync-enabled' => boolval($params['blocklist-sync-enabled']),
            'blocklist-sync-frequency' => $params['blocklist-sync-frequency'],
            'blocklist-url' => $params['blocklist-url'],
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getBlocklistsXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get locations and files settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getFilesAndLocationsSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getFilesAndLocationsXpath());

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getFilesAndLocationsXpath()
            );
        }

        // Modify the result data.
        $object['incomplete-dir-enabled'] = boolval($object['incomplete-dir-enabled']);
        $object['watch-dir-enabled'] = boolval($object['watch-dir-enabled']);
        $object['preallocation'] = intval($object['preallocation']);
        $object['rename-partial-files'] = boolval($object['rename-partial-files']);
        $object['start-added-torrents'] = boolval($object['start-added-torrents']);
        $object['trash-original-torrent-files'] = boolval($object['trash-original-torrent-files']);
        $object['umask'] = intval($object['umask']);

        return $object;
    }

    /**
     * Set locations and files settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws E_CONFIG_SET_OBJECT_FAILED
     */
    public function setFilesAndLocationsSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "download.sharedfolderref": {
                        '.$GLOBALS['OMV_JSONSCHEMA_UUID_NONE'].'
                    },
                    "download-dir": {
                        "type": "string"
                    },
                    "incomplete-dir-enabled": {
                        "type": "boolean"
                    },
                    "incomplete.sharedfolderref": {
                        '.$GLOBALS['OMV_JSONSCHEMA_UUID_NONE'].'
                    },
                    "incomplete-dir": {
                        "type": "string"
                    },
                    "preallocation": {
                        "type": "integer",
                        "enum": [0, 1, 2]
                    },
                    "rename-partial-files": {
                        "type": "boolean"
                    },
                    "start-added-torrents": {
                        "type": "boolean"
                    },
                    "trash-original-torrent-files": {
                        "type": "boolean"
                    },
                    "umask": {
                        "type": "integer"
                    },
                    "watch.sharedfolderref": {
                        '.$GLOBALS['OMV_JSONSCHEMA_UUID_NONE'].'
                    },
                    "watch-dir": {
                        "type": "string"
                    },
                    "watch-dir-enabled": {
                        "type": "boolean"
                    }
                }
            }'
        );

        // Prepare the configuration data.
        $object = [
            'download.sharedfolderref' => $params['download.sharedfolderref'],
            'download-dir' => $params['download-dir'],
            'incomplete-dir-enabled' => boolval($params['incomplete-dir-enabled']),
            'incomplete.sharedfolderref' => $params['incomplete.sharedfolderref'],
            'incomplete-dir' => $params['incomplete-dir'],
            'preallocation' => $params['preallocation'],
            'rename-partial-files' => boolval($params['rename-partial-files']),
            'start-added-torrents' => boolval($params['start-added-torrents']),
            'trash-original-torrent-files' => boolval($params['trash-original-torrent-files']),
            'umask' => intval($params['umask']),
            'watch-dir-enabled' => boolval($params['watch-dir-enabled']),
            'watch.sharedfolderref' => $params['watch.sharedfolderref'],
            'watch-dir' => $params['watch-dir'],
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getFilesAndLocationsXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get queuing settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getQueuingSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getQueuingXpath());

        if (is_null($object)) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED, $this->getQueuingXpath());
        }

        // Modify the result data.
        $object['download-queue-enabled'] = boolval($object['download-queue-enabled']);
        $object['download-queue-size'] = intval($object['download-queue-size']);
        $object['queue-stalled-enabled'] = boolval($object['queue-stalled-enabled']);
        $object['queue-stalled-minutes'] = intval($object['queue-stalled-minutes']);
        $object['seed-queue-enabled'] = boolval($object['seed-queue-enabled']);
        $object['seed-queue-size'] = intval($object['seed-queue-size']);

        return $object;
    }

    /**
     * Set queueing settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setQueuingSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "download-queue-enabled": {
                        "type": "boolean"
                    },
                    "download-queue-size": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "queue-stalled-enabled": {
                        "type": "boolean"
                    },
                    "queue-stalled-minutes": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "seed-queue-enabled": {
                        "type": "boolean"
                    },
                    "seed-queue-size": {
                        "type": "integer",
                        "minimum": 0
                    }
                }
            }'
        );

        // Prepare the configuration data.
        $object = [
            'download-queue-enabled' => boolval($params['download-queue-enabled']),
            'download-queue-size' => $params['download-queue-size'],
            'queue-stalled-enabled' => boolval($params['queue-stalled-enabled']),
            'queue-stalled-minutes' => $params['queue-stalled-minutes'],
            'seed-queue-enabled' => boolval($params['seed-queue-enabled']),
            'seed-queue-size' => $params['seed-queue-size'],
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getQueuingXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get peer settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getPeerSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getPeerXpath());

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getPeerXpath()
            );
        }

        $object['peer-limit-global'] = intval($object['peer-limit-global']);
        $object['peer-limit-per-torrent'] = intval($object['peer-limit-per-torrent']);

        $object['peer-port'] = intval($object['peer-port']);
        $object['peer-port-random-high'] = intval($object['peer-port-random-high']);
        $object['peer-port-random-low'] = intval($object['peer-port-random-low']);
        $object['peer-port-random-on-start'] = boolval($object['peer-port-random-on-start']);
        $object['port-forwarding-enabled'] = boolval($object['port-forwarding-enabled']);

        return $object;
    }

    /**
     * Set peer settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setPeerSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "bind-address-ipv4": {
                        "type": "string",
                        "format": "regex",
                        "pattern": "/^[0-9]{1,3}\\\.[0-9]{1,3}\\\.[0-9]{1,3}\\\.[0-9]{1,3}/"
                    },
                    "bind-address-ipv6": {
                        "type": "string"
                    },
                    "peer-limit-global": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "peer-limit-per-torrent": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "peer-socket-tos": {
                        "type": "string",
                        "enum": [
                            "default",
                            "lowcost",
                            "throughput",
                            "lowdelay",
                            "reliability"
                        ]
                    },
                    "peer-port": {
                        "type": "integer",
                        "minimum": 1024,
                        "maximum": 65535
                    },
                    "peer-port-random-low": {
                        "type": "integer",
                        "minimum": 1024,
                        "maximum": 65535
                    },
                    "peer-port-random-high": {
                        "type": "integer",
                        "minimum": 1024,
                        "maximum": 65535
                    },
                    "peer-port-random-on-start": {
                        "type": "boolean"
                    },
                    "port-forwarding-enabled": {
                        "type": "boolean"
                    }
                }
            }'
        );

        // Prepare the configuration data.
        $object = [
            'bind-address-ipv4' => $params['bind-address-ipv4'],
            'bind-address-ipv6' => $params['bind-address-ipv6'],
            'peer-limit-global' => $params['peer-limit-global'],
            'peer-limit-per-torrent' => $params['peer-limit-per-torrent'],
            'peer-socket-tos' => $params['peer-socket-tos'],

            'peer-port' => $params['peer-port'],
            'peer-port-random-high' => $params['peer-port-random-high'],
            'peer-port-random-low' => $params['peer-port-random-low'],
            'peer-port-random-on-start' => boolval($params['peer-port-random-on-start']),
            'port-forwarding-enabled' => boolval($params['port-forwarding-enabled']),
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getPeerXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get RPC settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getRpcSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getRpcXpath());
        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getRpcXpath()
            );
        }

        // Modify the result object.
        $object['rpc-authentication-required'] = boolval($object['rpc-authentication-required']);
        $object['rpc-enabled'] = boolval($object['rpc-enabled']);
        $object['rpc-port'] = intval($object['rpc-port']);

        return $object;
    }

    /**
     * Set RPC settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setRpcSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "rpc-authentication-required": {
                        "type": "boolean"
                    },
                    "rpc-enabled": {
                        "type": "boolean"
                    },
                    "rpc-password": {
                        "type": "string"
                    },
                    "rpc-port": {
                        "type": "integer",
                        "minimum": 1024,
                        "maximum": 65535
                    },
                    "rpc-username": {
                        "type": "string"
                    }
                }
            }'
        );

        // Prepare the configuration data.
        $object = [
            'rpc-authentication-required' => boolval($params['rpc-authentication-required']),
            'rpc-enabled' => boolval($params['rpc-enabled']),
            'rpc-password' => $params['rpc-password'],
            'rpc-port' => $params['rpc-port'],
            'rpc-username' => $params['rpc-username'],
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getRpcXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }

    /**
     * Get scheduling settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function getSchedulingSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Get the configuration object.
        $object = $xmlConfig->get($this->getSchedulingXpath());

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getSchedulingXpath()
            );
        }

        $object['alt-speed-time-enabled'] = boolval($object['alt-speed-time-enabled']);
        $object['alt-speed-time-begin'] = intval($object['alt-speed-time-begin']);
        $object['begin-hour'] = intval($object['alt-speed-time-begin'] / 60);
        $object['begin-minute'] = ($object['alt-speed-time-begin'] % 60);
        $object['alt-speed-time-end'] = intval($object['alt-speed-time-end']);
        $object['end-hour'] = intval($object['alt-speed-time-end'] / 60);
        $object['end-minute'] = ($object['alt-speed-time-end'] % 60);

        // Calculate the enabled days. If the correct bit is set the day is
        // enabled.
        $object['days-sunday'] = ($object['alt-speed-time-day'] & 1) === 1;
        $object['days-monday'] = ($object['alt-speed-time-day'] & 2) === 2;
        $object['days-tuesday'] = ($object['alt-speed-time-day'] & 4) === 4;
        $object['days-wednesday'] = ($object['alt-speed-time-day'] & 8) === 8;
        $object['days-thursday'] = ($object['alt-speed-time-day'] & 16) === 16;
        $object['days-friday'] = ($object['alt-speed-time-day'] & 32) === 32;
        $object['days-saturday'] = ($object['alt-speed-time-day'] & 64) === 64;

        $object['idle-seeding-limit'] = intval($object['idle-seeding-limit']);
        $object['idle-seeding-limit-enabled'] = boolval($object['idle-seeding-limit-enabled']);
        $object['ratio-limit'] = floatval($object['ratio-limit']);
        $object['ratio-limit-enabled'] = boolval($object['ratio-limit-enabled']);

        return $object;
    }

    /**
     * Set scheduling settings.
     *
     * @param array $params
     * @param array $context
     * @return array
     *
     * @throws OMVException
     */
    public function setSchedulingSettings($params, $context)
    {
        global $xmlConfig;

        // Validate the RPC caller context.
        $this->validateMethodContext($context, [
            'role' => OMV_ROLE_ADMINISTRATOR,
        ]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams(
            $params,
            '{
                "type": "object",
                "properties": {
                    "alt-speed-time-enabled": {
                        "type": "boolean"
                    },
                    "begin-hour": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 23
                    },
                    "begin-minute": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 59
                    },
                    "end-hour": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 23
                    },
                    "end-minute": {
                        "type": "integer",
                        "minimum": 0,
                        "maximum": 59
                    },
                    "days-sunday": {
                        "type": "boolean"
                    },
                    "days-monday": {
                        "type": "boolean"
                    },
                    "days-tuesday": {
                        "type": "boolean"
                    },
                    "days-wednesday": {
                        "type": "boolean"
                    },
                    "days-thursday": {
                        "type": "boolean"
                    },
                    "days-friday": {
                        "type": "boolean"
                    },
                    "days-saturday": {
                        "type": "boolean"
                    },
                    "idle-seeding-limit": {
                        "type": "integer",
                        "minimum": 0
                    },
                    "idle-seeding-limit-enabled": {
                        "type": "boolean"
                    },
                    "ratio-limit": {
                        "type": "number",
                        "minimum": 0
                    },
                    "ratio-limit-enabled": {
                        "type": "boolean"
                    }
                }
            }'
        );

        $params['alt-speed-time-day'] = 0;

        if (boolval($params['days-sunday']) === true) {
            $params['alt-speed-time-day'] += 1;
        }

        if (boolval($params['days-monday']) === true) {
            $params['alt-speed-time-day'] += 2;
        }

        if (boolval($params['days-tuesday']) === true) {
            $params['alt-speed-time-day'] += 4;
        }

        if (boolval($params['days-wednesday']) === true) {
            $params['alt-speed-time-day'] += 8;
        }

        if (boolval($params['days-thursday']) === true) {
            $params['alt-speed-time-day'] += 16;
        }

        if (boolval($params['days-friday']) === true) {
            $params['alt-speed-time-day'] += 32;
        }

        if (boolval($params['days-saturday']) === true) {
            $params['alt-speed-time-day'] += 64;
        }

        // Prepare the configuration data.
        $object = [
            'alt-speed-time-enabled' => boolval($params['alt-speed-time-enabled']),
            'alt-speed-time-begin' => intval(($params['begin-hour'] * 60) + $params['begin-minute']),
            'alt-speed-time-end' => intval(($params['end-hour'] * 60) + $params['end-minute']),
            'alt-speed-time-day' => $params['alt-speed-time-day'],
            'idle-seeding-limit' => $params['idle-seeding-limit'],
            'idle-seeding-limit-enabled' => boolval($params['idle-seeding-limit-enabled']),
            'ratio-limit' => $params['ratio-limit'],
            'ratio-limit-enabled' => boolval($params['ratio-limit-enabled']),
        ];

        // Set the configuration object.
        if ($xmlConfig->replace($this->getSchedulingXpath(), $object) === false) {
            throw new OMVException(OMVErrorMsg::E_CONFIG_SET_OBJECT_FAILED);
        }

        // Notify about the configuration changes.
        $dispatcher = &OMVNotifyDispatcher::getInstance();
        $dispatcher->notify(OMV_NOTIFY_MODIFY, $this->eventMessagePath, $object);

        return $object;
    }
}

// Register the RPC service.
$rpcServiceMgr = &OMVRpcServiceMgr::getInstance();
$rpcServiceMgr->registerService(new OMVRpcServiceTransmissionBt());
