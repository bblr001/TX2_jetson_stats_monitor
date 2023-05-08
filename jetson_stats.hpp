// Code Source: https://github.com/zhangxianbing/jetson_stats
#pragma once

#include <sys/stat.h>
//#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>
using namespace std;
namespace jetson_stats {

char currtime[128] = {};
std::string getDateTime(){
    time_t now;
    struct tm * tm_now;
    time(&now);
    tm_now = localtime(&now);
    
    strftime(currtime, 128, "%Y-%m-%d %H:%M:%S", tm_now);
    return currtime;
}

static std::vector<std::string> tegrastats_path{
    "/usr/bin/tegrastats",
    "/home/ubuntu/tegrastats",
    "/home/nvidia/tegrastats"
};

static std::vector<std::string> rvcp_path{
/*ref content
forums.developer.nvidia.com/t/jetson-tx1-ina226-power-monitor-with-i2c-interface/43819/7
forums.developer.nvidia.com/t/jetson-tx2-ina226-power-monitor-with-i2c-interface/48754/9
*/
"/sys/bus/i2c/drivers/ina3221x/0-0040/iio_device",
"/sys/bus/i2c/drivers/ina3221x/0-0041/iio_device",
"/sys/bus/i2c/drivers/ina3221x/0-0042/iio_device",
"/sys/bus/i2c/drivers/ina3221x/0-0043/iio_device"
};

static std::vector<std::string> temp_path{
"/sys/devices/virtual/thermal/thermal_zone0",
"/sys/devices/virtual/thermal/thermal_zone1",
"/sys/devices/virtual/thermal/thermal_zone2",
"/sys/devices/virtual/thermal/thermal_zone3",
"/sys/devices/virtual/thermal/thermal_zone4",
"/sys/devices/virtual/thermal/thermal_zone5",
"/sys/devices/virtual/thermal/thermal_zone6",
"/sys/devices/virtual/thermal/thermal_zone7",
};

static std::regex RAM_RE{R"(RAM (\d+)\/(\d+)MB)"};
static std::regex UTIL_RE{R"(GR3D(_FREQ?) ([0-9]+)%)"};
static std::regex CPU_RE{R"(CPU \[(.*?)\])"};
static std::regex VAL_FRE_RE{R"(\b(\d+)%@(\d+))"};

static bool is_file(const std::string& path) {
    struct stat s {};
    if (stat(path.c_str(), &s) == 0) {
        return s.st_mode & S_IFREG;
    }
    return false;
}

map<string, int> lookup_TX2_temprature()
{
/*
look up board device temprature
*/
	map<string, int> mapres={};   
	for(int i = 0; i<8; i++) // temp
	{
		string cmd = temp_path[i] + "/" + "type";	
		cmd = "cat "+ cmd;
		std::unique_ptr<FILE, decltype(&pclose)> pipefile(popen(cmd.c_str(), "r"), pclose);
		char buff[32] ={};
		fgets(buff, 32, pipefile.get());		
		string temp_type = buff;
		temp_type.erase(temp_type.find_last_not_of("\n") + 1);

		cmd = temp_path[i] + "/" + "temp";	
		cmd = "cat "+ cmd;
		std::unique_ptr<FILE, decltype(&pclose)> pipefile_temp(popen(cmd.c_str(), "r"), pclose);
		
		fgets(buff, 32, pipefile_temp.get());
		int temp_val = atoi(buff) ;		
		mapres[temp_type] = temp_val;
	}
	return mapres;
}

map<string, vector<int>> lookup_current_power_voltage()
{
/*
look up current, power, voltage from INA3221x through I2C
*/
	map<string, vector<int> > mapres={};
	string railnames[3] = {"rail_name_0", "rail_name_1", "rail_name_2"};
	string rail0[3] = {"in_current0_input", "in_power0_input", "in_voltage0_input"};
	string rail1[3] = {"in_current1_input", "in_power1_input", "in_voltage1_input"};
	string rail2[3] = {"in_current2_input", "in_power2_input", "in_voltage2_input"};

	//rail lookup
	for(int i = 0; i<4; i++) // i2cpath
	{
		for(int j = 0; j<3; j++) // railnames
		{
			string cmd = rvcp_path[i] + "/" + railnames[j];
			cmd = "cat "+ cmd;
			std::unique_ptr<FILE, decltype(&pclose)> pipefile(popen(cmd.c_str(), "r"), pclose);
			char buff[32] ={};
			fgets(buff, 32, pipefile.get());
			
			string rail_key = buff;                        
			rail_key.erase(rail_key.find_last_not_of("\n") + 1);
			
			mapres[rail_key] = {};	

			for(int k=0; k<3;k++) // railN
			{	
				if (j==0)
				   cmd = rvcp_path[i] + "/" + rail0[k];
                                else if(j == 1)
				   cmd = rvcp_path[i] + "/" + rail1[k];
				else if (j==2)
				   cmd = rvcp_path[i] + "/" + rail2[k];
				cmd = "cat " + cmd;
				std::unique_ptr<FILE, decltype(&pclose)> pipefile_rails(popen(cmd.c_str(), "r"), pclose);
                                memset(buff, 0, 32);
				fgets(buff, 32, pipefile_rails.get());
				
				int val = atoi(buff) ;
				mapres[rail_key].push_back(val)	;
			}	
		
		}
		
	}
	return mapres;
}

static inline std::vector<std::string> split_string(const std::string& s, const std::string& c) {
    std::vector<std::string> v;
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2) {
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length()) v.push_back(s.substr(pos1));
    return v;
}

static inline std::map<std::string, int> val_fraq(const std::string& val) {
    if (val.find("@") != std::string::npos) {
        std::smatch sm;
        if (std::regex_search(val, sm, VAL_FRE_RE)) {
            if (sm.size() == 3) {
                return {{"val", std::stoi(sm[1])}, {"frq", std::stoi(sm[2]) * 1000}};
            }
            return {};
        }
    }
    return {{"val", std::stoi(val)}};
}

string lookup_tegrastats() {
/*
look up tegrastats and return result
*/
    std::string cmd;
    for (auto&& p : tegrastats_path) {
        if (is_file(p)) {
            cmd = p;
            break;
        }
    }

    if (cmd.empty()) {
        throw std::runtime_error("tegrastats is not available on this board");
    }
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe_file(popen(cmd.c_str(), "r"), pclose);
    char line[512];
    if (fgets(line, 512, pipe_file.get()) == nullptr) {
        return "tegrastats error";
    };

    string tegrasts = line;
    tegrasts.erase(tegrasts.find_last_not_of("\n") + 1);
    return tegrasts;
};


static void get_info() {
    string tegrasts = lookup_tegrastats();
    map<string, vector<int>> res_cpv = lookup_current_power_voltage();    
    map<string, vector<int>>::iterator it = res_cpv.begin();
    
    string cpv = " CPV [";

    for(;it!= res_cpv.end();it++)
    {
	char cpv_[256]={};
        sprintf(cpv_, "%s current:%dmA power:%dmV voltage:%.2fmW ",it->first.c_str(), it->second[0], it->second[1], it->second[2]/1000.0);
	cpv += cpv_;
    }
    cpv +="] ";

    string temp = "TEMP [";

    map<string, int> res_temp = lookup_TX2_temprature();
    map<string, int>::iterator it_temp = res_temp.begin();
    for(;it_temp!= res_temp.end();it_temp++)
    {		
	char temp_[128]={};
        sprintf(temp_, "%s: %.2f ", it_temp->first.c_str(), it_temp->second/1000.0);
	temp += temp_;
    }
    temp += "]";
    std::string res = getDateTime() + " " + tegrasts + cpv + temp;
    std::cout<<res<<std::endl;
	
}
}  // namespace jetson_stats
