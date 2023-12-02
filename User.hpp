/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenses- <amenses-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 17:15:32 by amenses-          #+#    #+#             */
/*   Updated: 2023/11/30 18:30:38 by amenses-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

class User
{
    private:
        /* 
        socket
        nickname
        username
        password
        channels (socket list?)
        operator (bool, yes/no)
        status (bool, online/offline)
        
        */
    public:
        User();
        ~User();
        User(User const & src);
        User & operator=(User const & src);
        
        // getters: isoperator, isonline, getchannels, getsocket, getnickname, getusername, getpassword
        // setters
        // methods
};

#endif
