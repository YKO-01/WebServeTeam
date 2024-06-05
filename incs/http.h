/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-mhar <ael-mhar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 09:11:09 by ael-mhar          #+#    #+#             */
/*   Updated: 2024/06/04 13:11:40 by ael-mhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef http_h
# define http_h

# define MAX_PORT 65535

# define HTTP_MAX_URI 1024    /* Max length of URI string */
# define HTTP_MAX_HOST 256    /* Max length of hostname string */
# define HTTP_MAX_BUFFER 2048 /* Max length of data buffer */
# define HTTP_MAX_VALUE 256   /* Max header field value length */

# include <iostream>

/*
 * HTTP state values...
 */

typedef enum /* States are server-oriented */
{
	HTTP_WAITING,   /* Waiting for command */
	HTTP_OPTIONS,   /* OPTIONS command, waiting for blank line */
	HTTP_GET,       /* GET command, waiting for blank line */
	HTTP_GET_SEND,  /* GET command, sending data */
	HTTP_HEAD,      /* HEAD command, waiting for blank line */
	HTTP_POST,      /* POST command, waiting for blank line */
	HTTP_POST_RECV, /* POST command, receiving data */
	HTTP_POST_SEND, /* POST command, sending data */
	HTTP_PUT,       /* PUT command, waiting for blank line */
	HTTP_PUT_RECV,  /* PUT command, receiving data */
	HTTP_DELETE,    /* DELETE command, waiting for blank line */
	HTTP_TRACE,     /* TRACE command, waiting for blank line */
	HTTP_CLOSE,     /* CLOSE command, waiting for blank line */
	HTTP_STATUS,    /* Command complete, sending status */
}				State;

/*
 * HTTP method values...
 */

typedef enum
{
	GET,
	PUT,
	POST,
	HEAD,
	TRACE,
	PATCH,
	DELETE,
	CONNECT,
	OPTIONS
}				Method;

/*
 * HTTP Uri structure...
 */

struct			Uri
{
	std::string scheme;
	std::string host;
	std::string port;
	std::string resource;
	std::string query;
	std::string fragment;
};

/*
 * HTTP version structure...
 */

struct			Version
{
	uint16_t	major;
	uint16_t	minor;
};

/*
 * HTTP keep-alive values...
 */

typedef enum
{
	HTTP_KEEPALIVE_OFF,
	HTTP_KEEPALIVE_ON
}				http_keepalive_t;

/*
 * HTTP transfer encoding values...
 */

typedef enum
{
	HTTP_ENCODE_LENGTH,  /* Data is sent with Content-Length */
	HTTP_ENCODE_CHUNKED, /* Data is chunked */
	HTTP_ENCODE_NOT_IMPLEMENTED
}				http_encoding_t;

/*
 * HTTP authentication types...
 */

typedef enum
{
	HTTP_AUTH_NONE,        /* No authentication in use */
	HTTP_AUTH_BASIC,       /* Basic authentication in use */
	HTTP_AUTH_MD5,         /* Digest authentication in use */
	HTTP_AUTH_MD5_SESS,    /* MD5-session authentication in use */
	HTTP_AUTH_MD5_INT,     /* Digest authentication in use for body */
	HTTP_AUTH_MD5_SESS_INT /* MD5-session authentication in use for body */
}				http_auth_t;

/*
 * HTTP status codes...
 */

typedef enum
{
		HTTP_CONTINUE = 100,      /* Everything OK, keep going... */
		HTTP_SWITCHING_PROTOCOLS, /* HTTP upgrade to TLS/SSL */

		HTTP_OK = 200,         
			/* OPTIONS/GET/HEAD/POST/TRACE command was successful */
		HTTP_CREATED,           /* PUT command was successful */
		HTTP_ACCEPTED,          /* DELETE command was successful */
		HTTP_NOT_AUTHORITATIVE, /* Information isn't authoritative */
		HTTP_NO_CONTENT,        /* Successful command, no new data */
		HTTP_RESET_CONTENT,     /* Content was reset/recreated */
		HTTP_PARTIAL_CONTENT,   /* Only a partial file was recieved/sent */

		HTTP_MULTIPLE_CHOICES = 300, /* Multiple files match request */
		HTTP_MOVED_PERMANENTLY,      /* Document has moved permanently */
		HTTP_FOUND,                  /* Document has moved temporarily */
		HTTP_SEE_OTHER,              /* See this other link... */
		HTTP_NOT_MODIFIED,           /* File not modified */
		HTTP_USE_PROXY,              /* Must use a proxy to access this URI */

		HTTP_BAD_REQUEST = 400,      /* Bad request */
		HTTP_UNAUTHORIZED,           /* Unauthorized to access host */
		HTTP_PAYMENT_REQUIRED,       /* Payment required */
		HTTP_FORBIDDEN,              /* Forbidden to access this URI */
		HTTP_NOT_FOUND,              /* URI was not found */
		HTTP_METHOD_NOT_ALLOWED,     /* Method is not allowed */
		HTTP_NOT_ACCEPTABLE,         /* Not Acceptable */
		HTTP_PROXY_AUTHENTICATION,   /* Proxy Authentication is Required */
		HTTP_REQUEST_TIMEOUT,        /* Request timed out */
		HTTP_CONFLICT,               /* Request is self-conflicting */
		HTTP_GONE,                   /* Server has gone away */
		HTTP_LENGTH_REQUIRED,       
			/* A content length or encoding is required */
		HTTP_PRECONDITION,           /* Precondition failed */
		HTTP_REQUEST_TOO_LARGE,      /* Request entity too large */
		HTTP_URI_TOO_LONG,           /* URI too long */
		HTTP_UNSUPPORTED_MEDIATYPE, 
			/* The requested media type is unsupported */
		HTTP_UPGRADE_REQUIRED = 426, /* Upgrade to SSL/TLS required */

		HTTP_SERVER_ERROR = 500,  /* Internal server error */
		HTTP_NOT_IMPLEMENTED,     /* Feature not implemented */
		HTTP_BAD_GATEWAY,         /* Bad gateway */
		HTTP_SERVICE_UNAVAILABLE, /* Service is unavailable */
		HTTP_GATEWAY_TIMEOUT,     /* Gateway connection timed out */
		HTTP_NOT_SUPPORTED        /* HTTP version not supported */
} Status;

/*
 * HTTP field names...
 */

typedef enum
{
	HTTP_FIELD_EXTRA = -1,        /* field is extra */
	HTTP_FIELD_HOST,              /* Host field (request) */
	HTTP_FIELD_USER_AGENT,        /* User-Agent field (request) */
	HTTP_FIRLD_REFERER,           /* Referer field (request) */
	HTTP_FIELD_ACCEPT,           
		/* Accept field specifies the data types the client can handle (request) */
	HTTP_FIELD_ACCEPT_ENCODING,  
		/* Accept-Encoding specifies the encoding the client can handle (request) */
	HTTP_FIELD_ACCEPT_LANGAUAGE,  /* Accept-Language (request) */
	HTTP_FIELD_CONTENT_TYPE,     
		/* Content-Type field type of body data (representation) */
	HTTP_FIELD_CONTENT_LENGTH,   
		/* Content-Length field size of body data (representation) */
	HTTP_FIELD_CONTENT_ENCOGING, 
		/* Content-Encoding field encoding of body data (representation) */
	HTTP_FIELD_CONTENT_LANGUAGE, 
		/* Content-Language field describe the language(s) intended for the audience (representation) */
	HTTP_FIELD_CONTENT_LOCATION, 
		/* Content-Location field indicates the direct URL to use to access the resource (representation) */
	HTTP_FIELD_COOKIE,            /* Cookie send cookies to server (cookies) */
	HTTP_FIELD_AUTHORIZATION,     /* Authorization field (authentication) */
	HTTP_FIELD_TRANSFER_ENCODING,
		/* Transfer-Encoding field specifies the form of encoding used to safely transfer the payload body (transfer coding) */
	HTTP_FIELD_CONNECTION,       
		/* Connection field keep-alive or closed (request) */
	HTTP_FIELD_KEEP_ALIVE,        /* Keep-Alive field */
	HTTP_FIELD_DATE,              /* Date field (general) */
	HTTP_FIELD_SERVER,            /* Server field (responce) */
	HTTP_FIELD_LOCATION,          /* Location field (redirection) */
	HTTP_FIELD_SET_COOKIE,        /* Set-Cookie field (responce) */
	HTTP_FIELD_WWW_AUTHENTICATE,  /* WWW-Authenticate field (authentication) */
	HTTP_FIELD_RETRY_AFTER        /* Retry-After field (responce) */
}				http_field_t;

/*
 * HTTP uri status...
 */

typedef enum http_uri_status
{
	HTTP_URI_STATUS_OVERFLOW = -6,     /* URI buffer is too long (error) */
	HTTP_URI_STATUS_BAD_RESOURCE = -5, /* Bad resource in URI (error) */
	HTTP_URI_STATUS_BAD_PORT = -4,     /* Bad port number in URI (error) */
	HTTP_URI_STATUS_BAD_HOST = -3,     /* Bad hostname in URI (error) */
	HTTP_URI_STATUS_BAD_SCHEME = -2,   /* Bad scheme in URI (error) */
	HTTP_URI_STATUS_BAD_URI = -1,      /* Bad/empty URI (error) */
	HTTP_URI_STATUS_OK = 0,            /* URI decoded OK */
}				http_uri_status_t;

#endif
