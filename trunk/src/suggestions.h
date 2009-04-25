/**
 * Artha - Free cross-platform open thesaurus
 * Copyright (C) 2009  Sundaram Ramaswamy, legends2k@yahoo.com
 *
 * Artha is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Artha is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Artha; if not, see <http://www.gnu.org/licenses/>.
 */


/*
	Header suggestions.h

	Dynamic loading of libenchant for spell checks using 
	GLib's GModule. Prototypes must match with those in 
	enchant.h and enchant-provider.h
*/

#ifndef SUGGESTIONS_H
#define SUGGESTIONS_H


#include <sys/types.h>
#include <gmodule.h>
#include "wni.h"


#define ENCHANT_FILE		"libenchant.so.1"
#define DICT_TAG_ENGLISH	"en"
#define DICT_ENGLISH_PREFIX	"en_"
#define	DICT_TAG_MAX_LENGTH	7


#define FUNC_BROKER_INIT	"enchant_broker_init"
#define FUNC_BROKER_FREE	"enchant_broker_free"
#define FUNC_BROKER_DICT_EXISTS	"enchant_broker_dict_exists"
#define FUNC_BROKER_LIST_DICTS	"enchant_broker_list_dicts"
#define FUNC_BROKER_REQ_DICT	"enchant_broker_request_dict"
#define FUNC_BROKER_FREE_DICT	"enchant_broker_free_dict"
#define FUNC_DICT_CHECK		"enchant_dict_check"
#define FUNC_DICT_SUGGEST	"enchant_dict_suggest"
#define FUNC_DICT_FREE_STRINGS	"enchant_dict_free_string_list"
#define FUNC_DICT_FREE_SUGGESTS	"enchant_dict_free_suggestions"


//Structure Prototypes

typedef struct str_enchant_broker EnchantBroker;
typedef struct str_enchant_dict   EnchantDict;

struct str_enchant_dict
{
	void *user_data;
	void *enchant_private_data;

	int (*check) (struct str_enchant_dict * me, const char *const word,
			  size_t len);

	/* returns utf8*/
	char **(*suggest) (struct str_enchant_dict * me,
			   const char *const word, size_t len,
			   size_t * out_n_suggs);
	
	void (*add_to_personal) (struct str_enchant_dict * me,
				 const char *const word, size_t len);
	
	void (*add_to_session) (struct str_enchant_dict * me,
				const char *const word, size_t len);
	
	void (*store_replacement) (struct str_enchant_dict * me,
				   const char *const mis, size_t mis_len,
				   const char *const cor, size_t cor_len);
	
	void (*add_to_exclude) (struct str_enchant_dict * me,
				 const char *const word, size_t len);
	
	void * _reserved[5];
};
	
struct str_enchant_provider
{
	void *user_data;
	void *enchant_private_data;
	EnchantBroker * owner;
	
	void (*dispose) (struct str_enchant_provider * me);
	
	EnchantDict *(*request_dict) (struct str_enchant_provider * me,
					  const char *const tag);
	
	void (*dispose_dict) (struct str_enchant_provider * me,
				  EnchantDict * dict);
	
	int (*dictionary_exists) (struct str_enchant_provider * me,
				  const char *const tag);

	/* returns utf8*/
	const char * (*identify) (struct str_enchant_provider * me);
	/* returns utf8*/
	const  char * (*describe) (struct str_enchant_provider * me);

	/* frees string lists returned by list_dicts and dict->sugges	EnchantBroker *spell_broker = NULL;
	EnchantDict *dict = NULL;t */
	void (*free_string_list) (struct str_enchant_provider * me,
				  char **str_list);

	char ** (*list_dicts) (struct str_enchant_provider * me,
							   size_t * out_n_dicts);

	void * _reserved[5];
};


//Function Prototypes

typedef void	(*EnchantDictDescribeFn)		(const char * const lang_tag, const char * const provider_name,
							 const char * const provider_desc, const char * const provider_file,
							 void * user_data);

G_BEGIN_DECLS


EnchantBroker*	(*enchant_broker_init)			(void) = NULL;
void		(*enchant_broker_free)			(EnchantBroker * broker) = NULL;


int		(*enchant_broker_dict_exists)		(EnchantBroker * broker, const char * const tag) = NULL;
void		(*enchant_broker_list_dicts)		(EnchantBroker * broker, EnchantDictDescribeFn fn, void * user_data) = NULL;
EnchantDict*	(*enchant_broker_request_dict)		(EnchantBroker * broker, const char *const tag) = NULL;
void		(*enchant_broker_free_dict)		(EnchantBroker * broker, EnchantDict * dict) = NULL;


int		(*enchant_dict_check)			(EnchantDict * dict, const char *const word, ssize_t len) = NULL;
char**		(*enchant_dict_suggest)			(EnchantDict * dict, const char *const word, ssize_t len, size_t * out_n_suggs) = NULL;
void		(*enchant_dict_free_string_list)	(EnchantDict * dict, char **string_list) = NULL;

// free suggestions is depricated, only if enchant_dict_free_string_list is not available, should it be used
//void		(*enchant_dict_free_suggestions)	(EnchantDict * dict, char **suggestions) = NULL;

char*		(*enchant_dict_get_error)		(EnchantDict * dict) = NULL;
char*		(*enchant_broker_get_error)		(EnchantBroker * broker) = NULL;


G_END_DECLS

// Global variables

GModule *mod_enchant = NULL;
EnchantBroker *enchant_broker = NULL;
EnchantDict *enchant_dict = NULL;

#endif

