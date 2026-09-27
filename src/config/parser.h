#ifndef PARSER_H
#define PARSER_H

#define DEFAULT_CONFIG_FILE "/etc/mynetd/mynetd.conf"
#define ADDRS_INHERIT NULL

// This is so scuffed
// Should separate these into headers

typedef enum services_t{
	SERV_ECHO,
	SERV_QOTD,
	SERV_TIME,
	SERV_DAYTIME,
	SERV_CHARGEN,
	SERV_DISCARD,
} services_t;

typedef enum log_levels_t{
	LEVEL_INHERIT,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
	LEVEL_FATAL,
} log_levels_t;

typedef enum rate_limits_t{
	LIMIT_INHERIT,
	LIMIT_NONE,
	LIMIT_LIGHT,
	LIMIT_MEDIUM,
	LIMIT_STRICT,
} rate_limits_t;

typedef enum transport_rules_t{
	TP_TCP_INHERIT = 0x01,
	TP_TCP_ENABLE = 0x02,
	TP_UDP_INHERIT = 0x04,
	TP_UDP_ENABLE = 0x08,
} transport_rules_t;

typedef struct addrlist_t{
	struct addrlist_t *next;
	struct sockaddr_storage addr;
} addrlist_t;

// Individual service configs

typedef struct service_echo_t{
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	uint16_t port;
} service_echo_t;

typedef struct service_qotd_t{
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	char *filepath;
	uint16_t port;
} service_qotd_t;

typedef struct service_time_t{
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	uint16_t port;
} service_time_t;

typedef enum daytime_type_t{
	ISO_8601,
} daytime_type_t;

typedef struct service_daytime_t{
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	enum daytime_type_t time_type;
	uint16_t port;
} service_daytime_t;

typedef struct service_chargen_t{
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	char *filepath;
	uint16_t port;
} service_chargen_t;

typedef struct service_discard_t{
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	uint16_t port;
} service_discard_t;

// Server level configs

typedef struct port_map_t{
	struct port_map_t *next;
	void *service_config;
	enum services_t service;
	uint16_t port;
} port_map_t;

typedef struct configs_t{
	struct port_map_t *services;
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
} configs_t;

int parse_config_file(char *path, configs_t **ret);

#endif
