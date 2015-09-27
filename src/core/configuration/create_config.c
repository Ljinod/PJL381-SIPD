/**
 * @file create_config.c
 * @brief   XXX Add brief description!
 * @author  Loudet Julien <loudet.julien@gmail.com>
 * @version 1.1
 * @date 2015-09
 */
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "create_config.h"
#include "configuration.h" /**< For the #define */
#include "../beans/my_info.h" /**< MyInfo_t structure */
#include "../tools/utils.h" /**< For the "get_user_input" function */


/**
 * @brief   Creates a configuration file for the user.
 * @details This function will ask the user the required information in order
 *          to use the SIPD (see ask_user_configuration_file for a detailed
 *          list of those information).
 *
 *          If the user cannot provide a public/private RSA pair of keys then
 *          a new pair is created and stored here: $HOME/.config/sipd/keys/
 *
 *          The configuration file is stored here:
 *          $HOME/.config/sipd/my_info.xml
 *
 *          At the same time the MyInfo_t structure is filled with the
 *          information provided.
 *
 *          ===================================================================
 *          TO BE DONE
 *          ===================================================================
 *          XXX Duplicate RSA keys for the tcell? Or the client and the tcell
 *          both use the same configuration file?
 *          XXX Verify if RSA keys do exist before adding them?
 *
 * @param config_file_path The path were the file will be created.
 *
 * @return my_info The structure holding the user's information.
 */
MyInfo_t* create_user_configuration_file(const char *config_file_path)
{
    /* The structure to hold the user's information */
    MyInfo_t *my_info = malloc(sizeof(MyInfo_t));

    /* The buffer to receive the user's input */
    char buffer[ARR_SIZE];
    memset(buffer, '\0', ARR_SIZE);

    /* The XML writer, to store the information in the file my_info.xml */
    int rc;
    xmlTextWriterPtr writer;
    writer = xmlNewTextWriterFilename(config_file_path, 0);
    if (writer == NULL)
    {
        fprintf(stderr, "testXmlwriterFilename: Error creating the xml "
                        "writer\n");
        goto error;
    }

    /* We start the XML document */
    rc = xmlTextWriterStartDocument(writer, NULL, XML_ENCODING, NULL);
    if (rc < 0)
    {
        fprintf(stderr, "testXmlwriterFilename: Error at "
                        "xmlTextWriterStartDocument\n");
        goto error;
    }

    /* The first element being the root of the xml, we call it MY_INFO */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "MY_INFO");
    if (rc < 0)
    {
        fprintf(stderr,
                "testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        goto error;
    }

    /* From here on, we collect the information */
    fprintf(stdout, "\nPLEASE FILL IN THE FOLLOWING REQUIRED INFORMATION:\n");

    /* Tcell ip address */
    get_user_input(buffer, ARR_SIZE, "\nThe IP address of your tcell: ");
    rc = xmlTextWriterWriteElement(writer, BAD_CAST TCELL_IP,
                                     BAD_CAST buffer);
    if (rc < 0) { goto error_write_attribute; }
    strcpy(my_info->my_tcell_ip, buffer); /* Update my_info */

    /* Tcell port */
    get_user_input(buffer, ARR_SIZE, "\nThe port of your tcell: ");
    rc = xmlTextWriterWriteElement(writer, BAD_CAST TCELL_PORT,
                                     BAD_CAST buffer);
    if (rc < 0) { goto error_write_attribute; }
    strcpy(my_info->my_tcell_port, (buffer)); /* Update my_info */

    /* User id */
    get_user_input(buffer, ARR_SIZE, "\nEnter your name (this field will be "
                                      "used to generate your unique \n"
                                      "global id): ");
    rc = xmlTextWriterWriteElement(writer, BAD_CAST USER_ID,
                                     BAD_CAST buffer);
    if (rc < 0) { goto error_write_attribute; }
    strcpy(my_info->my_gid, buffer); /* Update my_info */

    /* RSA keys */
    get_user_input(buffer, ARR_SIZE, "\nDo you have an RSA key pair that you "
                                      "would like to associate to your \n"
                                      "tcell? If not a new one will be created"
                                      "\n[y/n]: ");
    if(buffer[0] == 'y' || buffer[0] == 'Y')
    {
        fprintf(stdout, "Enter their absolute paths:\n");
        get_user_input(buffer, ARR_SIZE, "(public key absolute path): ");
        rc = xmlTextWriterWriteElement(writer, BAD_CAST PUB_KEY_PATH,
                                         BAD_CAST buffer);
        if (rc < 0) { goto error_write_attribute; }
        strcpy(my_info->my_public_key, buffer); /* Update my_info */

        get_user_input(buffer, ARR_SIZE, "(private key absolute path): ");
        rc = xmlTextWriterWriteElement(writer, BAD_CAST PRIV_KEY_PATH,
                                         BAD_CAST buffer);
        if (rc < 0) { goto error_write_attribute; }
        strcpy(my_info->my_private_key, buffer); /* Update my_info */
    }
    else
    {
        get_user_input(buffer, ARR_SIZE, "\nEnter a name for the RSA keys, "
                                          "\".pub\" will be appended for the\n"
                                          "public key: ");

        /* Generation of private key.
         * For more details about this step read the man pages of openssl(1)
         * and genrsa(1). */
        fprintf(stdout, "=== Generating the private key:\n");
        char priv_key_filename[ARR_SIZE], cmd[ARR_SIZE];
        sprintf(priv_key_filename, "%s/.config/sipd/keys/%s", getenv("HOME"),
                buffer);
        sprintf(cmd, "openssl genrsa -out %s 2048", priv_key_filename);
        /* Launch command to create the private key. */
        rc = system(cmd); if(rc != 0) { goto error; }
        /* Add the private key path to the XML. */
        rc = xmlTextWriterWriteElement(writer, BAD_CAST PRIV_KEY_PATH,
                                         BAD_CAST priv_key_filename);
        if (rc < 0) { goto error_write_attribute; }
        strcpy(my_info->my_private_key, priv_key_filename); /* Update my_info */

        /* Generation of public key from private key.
         * For more details read the man pages of rsa(1). */
        fprintf(stdout, "=== Generating the public key:\n");
        char pub_key_filename[ARR_SIZE];
        memset(cmd, '\0', ARR_SIZE); /* wipe previous content */
        sprintf(pub_key_filename, "%s.pub", priv_key_filename);
        sprintf(cmd, "openssl rsa -in %s -pubout -out %s", priv_key_filename,
                pub_key_filename);
        /* Launch command to create the public key from the private one. */
        rc = system(cmd); if(rc != 0) { goto error; }
        /* Add the public key path to the XML. */
        rc = xmlTextWriterWriteElement(writer, BAD_CAST PUB_KEY_PATH,
                                         BAD_CAST pub_key_filename);
        if (rc < 0) { goto error_write_attribute; }
        strcpy(my_info->my_public_key, pub_key_filename); /* Update my_info */
    }

    /* End the XML document and free it. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        fprintf(stderr,
                "testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        goto error;
    }
    xmlFreeTextWriter(writer);

    /* If we arrived here it means that the generation of the configuration
     * file was successful. */
    fprintf(stdout, "\n\n\nOK: Configuration file successfully created.\n");
    return my_info;


error_write_attribute:
    fprintf(stderr,
            "testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");

error:
    fprintf(stderr, "ERROR: Aborting creation of configuration file.\n");

    /* Close and free resources */
    rc = xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
    free(my_info);

    /* Delete incomplete configuration file */
    if(remove(config_file_path) != 0)
    {
        print_error_info("Could not remove config file.");
    }
    return NULL;
}
