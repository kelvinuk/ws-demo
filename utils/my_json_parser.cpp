//
// Created by owner on 10/21/24.
//
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>

void printJson(boost::property_tree::ptree const& pt)
{
    using boost::property_tree::ptree;
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
        std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
        printJson(it->second);
    }
}

std::string getSessionToken(const std::string& response)
{
    try {
        std::stringstream ss;
        ss << response;
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type & v, pt.get_child("particles"))
                    {
                        assert(v.first.empty()); // array elements have no names
                        printJson(v.second);
                    }
        //status = std::string(res.base().at(http::field::));
        //cookie = std::string(res.base().at(http::field::set_cookie));

        //std::cout << "Cookie: " << cookie << std::endl;
        //std::cout << "Response data: " << res << std::endl;
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return "";
}
