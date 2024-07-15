/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:44:34 by amitcul           #+#    #+#             */
/*   Updated: 2024/07/15 20:41:44 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

class Server;
class Channel;

#include <string>
#include <queue>

#include "Server.hpp"

/*
  FLAGS
*/
#define REGISTERED      0b00000001
#define INVISIBLE		    0b00000010
#define RECEIVENOTICE	  0b00000100
#define RECEIVEWALLOPS	0b00001000
#define CHANOPERATOR		0b00010000
#define AWAY            0b00100000
#define PINGING         0b01000000
#define BREAK           0b10000000

/**
 * Utils functions
*/
std::queue<std::string> split2queue(const std::string& str, char sep, bool incl);


class User
{
  private:
    int socket_fd_;
    std::string host_;
    const std::string& server_name_;
    const Server* server_;
    std::string password_;
    std::string nickname_;
    std::string username_;
    unsigned char flags_;
    time_t time_of_registrations_;
    time_t time_of_last_action_;
    time_t time_after_pinging_;

    std::queue<std::string> messages_;
    std::vector<const Channel*> channels_;

    /**
     * Forbidden APIs
    */
    User();
    User(const User& user);
    User& operator=(const User& oth);

  public:
    User(int socket_fd, const std::string& host, const Server* server, std::string& server_name);
    ~User();

    /**
     * Getters
    */
    int get_socket_fd() const;
    const std::string& get_host() const;
    const Server* get_server() const;
    const std::string& get_server_name() const;
    const std::string& get_password() const;
    const std::string& get_nickname() const;
    const std::string& get_username() const;
    unsigned char get_flags() const;
    time_t get_time_of_registrations() const;
    time_t get_time_of_last_action() const;
    time_t get_time_after_pinging() const;
    const std::vector<const Channel*>& get_channels() const;

    /**
     * Setters
    */
    void set_password(const std::string& password);
    void set_nickname(const std::string& nickname);
    void set_username(const std::string& username);
    void set_flag(unsigned char flag);
    void set_time_of_registrations();
    void set_time_after_pinging();
    void set_time_of_last_action();

    /**
     * Funcs
    */
    const std::queue<std::string>& get_message() const;
    void pop_message();
    void send_message(const std::string& message) const;
    int read_message();
    void reset_flag(unsigned char flag);
    std::string get_prefix() const;
    std::string get_quit_message() const;
};


#endif // USER_HPP
