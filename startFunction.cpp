#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
    string typeQuery;
    is >> typeQuery;
    q.stops.clear();
    if(typeQuery == "NEW_BUS"){
        int number;
        q.type = QueryType::NewBus;
        is >> q.bus;
        is >> number;
        string stop;
        for(int i(0); i < number; i++){
            is >> stop;
            q.stops.push_back(stop);
        }
    }else if(typeQuery == "BUSES_FOR_STOP"){
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    }else if(typeQuery == "STOPS_FOR_BUS"){
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    }else if(typeQuery == "ALL_BUSES"){
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    string info;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    os << r.info;
    return os;
}

struct StopsForBusResponse {
    string info;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    os << r.info;
    return os;
}

struct AllBusesResponse {
    string info;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    os << r.info;
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops[bus].clear();
        for (const string& stop : stops) {
            buses_to_stops[bus].push_back(stop);
            stops_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) {
        BusesForStopResponse rezult;
        if (stops_to_buses.count(stop) == 0) {
            rezult.info += "No stop";
        } else {
            for (const string& bus : stops_to_buses[stop]) {
                rezult.info += bus + " ";
            }
            rezult.info += "\n";
        }
        return rezult;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) {
        StopsForBusResponse rezult;
        if (buses_to_stops.count(bus) == 0) {
            rezult.info += "No bus";
        } else {
            for (const string& stop : buses_to_stops[bus]) {
                rezult.info += "Stop " + stop + ": ";
                if (stops_to_buses[stop].size() == 1) {
                    rezult.info += "no interchange";
                } else {
                    for (const string& other_bus : stops_to_buses[stop]) {
                        if (bus != other_bus) {
                            rezult.info += other_bus + " ";
                        }
                    }
                }
                rezult.info += "\n";
            }
        }
        return rezult;
    }

    AllBusesResponse GetAllBuses() {
        AllBusesResponse rezult;
        if (buses_to_stops.empty()) {
            rezult.info += "No buses";
        } else {
            for (const auto& bus_item : buses_to_stops) {
                rezult.info += "Bus " + bus_item.first + ": ";
                for (const string& stop : bus_item.second) {
                    rezult.info += stop + " ";
                }
                rezult.info += "\n";
            }
        }
        return rezult;
    }
private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;
};

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}
