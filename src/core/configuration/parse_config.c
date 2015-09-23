/**
 * @file    parse_config.c
 * @brief 
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date    2015-09
 *
 * @details (last edited by Loudet Julien on 2015/09)
 *
 *          Parses a configuration file. The configuration file, for now, has
 *          to follow a certain structure that is why it is STRONGLY advised to
 *          first generate a default configuration file and then copy/modify it
 *          according to your needs so that the structure stays intact.
 *
 *          The structure is as following:
 *
 *          <MY_INFO>
 *              <tcell_ip>127.0.0.1</tcell_ip>
 *              <tcell_port>5678</tcell_port>
 *              <id>julien</id>
 *              <priv_key_path>$HOME/.config/sipd/keys/priv_key</priv_key_path>
 *              <pub_key_path>$HOME/.config/sipd/keys/pub_key</pub_key_path>
 *          </MY_INFO>
 *
 *          NOTES: (*) The order of the parameters does not matter.
 *                 (*) The spelling and case of the parameters MUST be the 
 *                     SAME as above. See the header file "configuration.h".
 *                 (*) There MUST be NO SPACES in the values you provide.
 *                 (*) Other parameters will not be processed. Thus you can add
 *                     whatever information you want (for instance other path
 *                     for other RSA keys), only the parameter above will be
 *                     taken into account.
 *          
 *          ===================================================================
 *          TO BE DONE
 *          ===================================================================
 *          XXX Understand better the libxml2 api to be able to handle the
 *          error that a user could make in a custom configuration file.
 *          XXX Verify that the keys provided do exist, and are RSA public and
 *          private keys (if that is possible).
 */

#include <stdlib.h> /**< "atoi" function */
#include <string.h> /**< "strcpy" function */

#include <libxml/parser.h>
#include <libxml/xmlstring.h>
#include <libxml/tree.h>

#include "../beans/my_info.h"
#include "parse_config.h"
#include "configuration.h"


/* Process a node from the configuration file pointed to by the 
 * xmlTextReaderPtr */
void process_config_file(xmlDocPtr doc, xmlNodePtr root_element_node, 
                         MyInfo_t *my_info);


/**
 * @brief 
 *
 * @param config_file_path
 *
 * @return 
 */
MyInfo_t* parse_configuration_file(const char *config_file_path)
{
    /* Structure storing user's information */
    MyInfo_t *my_info = malloc(sizeof(MyInfo_t));

    /* The XML part: to parse a file we need a "doc" container as well as a
     * "root_element" to begin inspecting. */ 
    xmlDocPtr  doc               = NULL;
    xmlNodePtr root_element_node = NULL;

    if((doc = xmlReadFile(config_file_path, NULL, 0)) == NULL)
    {
        fprintf(stderr, "Could not parse file %s\n.", config_file_path);
        fprintf(stderr, "Exiting.\n");
        exit(-1);
    }

    root_element_node = xmlDocGetRootElement(doc);
    process_config_file(doc, root_element_node, my_info);
    return my_info;
}


/**
 * @brief Process the XML document and get the user's information.
 *
 * @details This function processes the XML document "doc", starting at its
 *          root node, "root_element_node" and extracts the information to
 *          fill in the user's information using the my_info structure.
 *
 *
 * @param doc A pointer to the XML document
 * @param root_node A pointer to the root element node
 * @param my_info The structure that will contain the user's information
 */
void process_config_file(xmlDocPtr doc, xmlNodePtr root_element_node, 
                         MyInfo_t *my_info)
{
    const xmlChar *node_name; 
    xmlChar       *text_node_value;
    xmlNodePtr     node, text_node;

    /* Extract all children nodes from the root node */
    node = root_element_node->xmlChildrenNode;

    /* As long as node is not null, we process! */
    while(node != NULL)
    {
        if((node_name = node->name) != NULL)
        {
            /* We get the text value associated to the node */
            text_node       = node->xmlChildrenNode;
            text_node_value = xmlNodeListGetString(doc, text_node, 1);

            /* We check that this value indeed interests us and if yes, we
             * copy its value in the my_info structure */
            if(xmlStrcmp((const xmlChar*) node_name,
                         (const xmlChar*) TCELL_PORT) == 0)
            {
                my_info->my_tcell_port = atoi((const char*) text_node_value);
            }
            else if(xmlStrcmp((const xmlChar*) node_name,
                              (const xmlChar*) TCELL_IP) == 0)
            {
                strcpy(my_info->my_tcell_ip, (const char*) text_node_value);
            }
            else if(xmlStrcmp((const xmlChar*) node_name,
                              (const xmlChar*) USER_ID) == 0)
            {
                strcpy(my_info->my_gid, (const char*) text_node_value);
            }
            else if(xmlStrcmp((const xmlChar*) node_name,
                              (const xmlChar*) PRIV_KEY_PATH) == 0)
            {
                strcpy(my_info->my_private_key, (const char*) text_node_value);
            }
            else if(xmlStrcmp((const xmlChar*) node_name,
                              (const xmlChar*) PUB_KEY_PATH) == 0)
            {
                strcpy(my_info->my_public_key, (const char*) text_node_value);
            }

            /* We can free the xmlChar since we copied everything */
            xmlFree(text_node_value);
        }

        node = node->next;
    }
}

