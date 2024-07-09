/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigBase.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nscheefe <nscheefe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 23:36:36 by nscheefe          #+#    #+#             */
/*   Updated: 2024/07/09 23:36:37 by nscheefe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// confBase.hpp
#pragma once

#include <iostream>

namespace Config {
    class confBase {
    protected:
        confBase(const confBase &) {}

    public:
        confBase() {}

        virtual ~confBase() {}

        virtual confBase &operator=(const confBase &) { return *this; }

        virtual void print() const {
            std::cout << "" << std::endl;
        }
    };
}