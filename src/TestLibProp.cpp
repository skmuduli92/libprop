
#include<bits/stdc++.h>

#include "formula.h"
#include "parse_util.h"
#include "formula_util.h"
#include "trace.h"

using namespace HyperPLTL;
using namespace std;


unsigned max(unsigned a, unsigned b){
    return a>b ? a : b;
}

//Function to read files and get the variables name by type = int or bool. 
vector<string> getVarsByType(string type){
    vector<string>varints;
    ifstream varfile;
    string x;
    varfile.open(type);
    while (varfile >> x) {
        if(x!="" && x!="\n")
            varints.push_back(x);
    }
    varfile.close();
    return varints;
}

// Function to read the traces from trace files. Map is of format Traces[TraceNumber][Variable_Name]<Vector of <time, value> pairs>
map<unsigned, map<string, vector<pair<unsigned, unsigned>>>> readFileTraces(){
  vector<string>varints = getVarsByType("IntVars");
  vector<string>varprops = getVarsByType("BoolVars");
  map<unsigned, map<string, vector<pair<unsigned, unsigned>>>>read_traces;

  ifstream varfile;
  for(unsigned i=0;i<varints.size();i++){
    unsigned tr, ti, val;
    string var = varints[i];
    varfile.open(varints[i]+".facts");
    string x;
    while (varfile >> x) {
        assert(x!="\n" && x!="" && x!="\0");
        tr = stoi(x);
        varfile >> x;
        ti = stoi(x);
        varfile >> x;
        val = stoi(x);
        if ( read_traces.find(tr) == read_traces.end() || read_traces[tr].find(var) == read_traces[tr].end())
            read_traces[tr][var] = vector<pair<unsigned, unsigned>>();
        read_traces[tr][var].push_back(make_pair(ti, val));
    }
    varfile.close();
  }
  for(unsigned i=0;i<varprops.size();i++){
    unsigned tr, ti, val;
    string var = varprops[i];
    varfile.open(varprops[i]+".facts");
    string x;
    while (varfile >> x) {
        assert(x!="\n" && x!="" && x!="\0");
        tr = stoi(x);
        varfile >> x;
        ti = stoi(x);
        varfile >> x;
        val = stoi(x);
        if ( read_traces.find(tr) == read_traces.end() || read_traces[tr].find(var) == read_traces[tr].end())
            read_traces[tr][var] = vector<pair<unsigned, unsigned>>();
        read_traces[tr][var].push_back(make_pair(ti, val));
    }
    varfile.close();
  }
  return read_traces;
}


// Function to read properties from file to verify
vector<string> read_formulas(string filename){
    string x;
    vector<string>result;
    ifstream varfile;
    varfile.open(filename);
    while (std::getline(varfile, x)) {
        if(x!=" " && x!="\n")
            result.push_back(x);
    }
    varfile.close();
    return result;
}


// Function to verify the formulas one by one picking pair of trace at a time
void ParseAll() {
    //Populate the variable names, and trace structures
    map<unsigned, map<string, vector<pair<unsigned, unsigned>>>> traces = readFileTraces();
    for(auto i: traces)
        for(auto j: i.second)
            sort(j.second.begin(), j.second.end());
    vector<string> formulas = read_formulas("Specifications");
    vector<string> negated_formulas = read_formulas("Negated_Specifications");
    vector<string> int_vars = getVarsByType("IntVars");
    vector<string> bool_vars = getVarsByType("BoolVars");

    
    assert(formulas.size() == negated_formulas.size() && formulas.size() > 0);
    assert(int_vars.size() > 0 || bool_vars.size() > 0);

    // File for writing satisfied formulas
    ofstream file_sat;
    file_sat.open("Satisfied_LibProp", ios::out | ios::trunc);

    // Populate the var map
    PVarMap varmap = std::make_shared<VarMap>();
    for (auto var: int_vars)
        varmap->addIntVar(var);
    for (auto var: bool_vars)
        varmap->addPropVar(var);

    // Loop over each property in the formulas list.
    for(unsigned it = 0; it < formulas.size(); it++){

        // Parsing the property
        PHyperProp property =  parse_formula(formulas[it], varmap);
        PHyperProp negated_property =  parse_formula(negated_formulas[it], varmap);


        bool has_negatives = false;
        bool satisfied = false;

        // Iterating over trace pairs.
        for(unsigned tr1=0;tr1<traces.size() && !has_negatives;tr1++){

            // Populating first trace
            unsigned traceend = INT_MAX;
            PTrace trace1(new Trace(bool_vars.size(), int_vars.size()));
            for(auto vars: int_vars){
                unsigned locTE = 0;
                unsigned xid = property->getVarId(vars);
                for(auto pr:traces[tr1][vars]){
                    trace1->updateTermValue(xid, pr.first, pr.second);
                    locTE = max(pr.first, locTE);
                }
                traceend = min(locTE, traceend);
            }
            for(auto vars: bool_vars){
                unsigned locTE = 0;
                unsigned xid = property->getPropId(vars);
                for(auto pr:traces[tr1][vars]){
                    assert(pr.second%2==pr.second);
                    trace1->updatePropValue(xid, pr.first, pr.second);
                    locTE = max(pr.first, locTE);
                }
                traceend = min(locTE, traceend);
            }

            // Iterating for second trace
            // ******************************************************************************
            // *********** Tr2 must start from 0 if the order of the pair matters ***********
            // ******************************************************************************
            for(unsigned tr2 = tr1+1; tr2<traces.size() && !has_negatives; tr2++){
                if(tr1 == tr2)
                    continue;
                
                // Populating second trace
                unsigned traceend_tr2 = INT_MAX;
                PTrace trace2(new Trace(bool_vars.size(), int_vars.size()));
                for(auto vars: int_vars){
                    unsigned locTE = 0;
                    unsigned xid = property->getVarId(vars);
                    for(auto pr:traces[tr2][vars]){
                        trace2->updateTermValue(xid, pr.first, pr.second);
                        locTE = max(pr.first, locTE);   
                    }
                    traceend_tr2 = min(locTE, traceend_tr2);
                }
                for(auto vars: bool_vars){
                    unsigned locTE = 0;
                    unsigned xid = property->getPropId(vars);
                    for(auto pr:traces[tr2][vars]){
                        assert(pr.second%2==pr.second);
                        trace2->updatePropValue(xid, pr.first, pr.second);
                        locTE = max(pr.first, locTE);   
                    }
                    traceend_tr2 = min(locTE, traceend_tr2);
                }

                // Handling unequal traces so that utility function runs till lower tracelength only.
                TraceList tracelist;
                if(traceend < traceend_tr2)
                    tracelist = TraceList({trace1, trace2});
                else
                    tracelist = TraceList({trace2, trace1});


                // Evaluate the property over tracelist
                bool negs = evaluateTraces(negated_property, tracelist);
                if(negs)
                    has_negatives = true;
                else{
                    bool poss = evaluateTraces(property, tracelist);
                    satisfied = satisfied || poss;
                }
            } // End of loop for Tr2
        }   // End of loop for Tr1
        if(satisfied && !has_negatives){
           file_sat<<formulas[it]<<endl;
        }
    } // End of loop for formulas
    file_sat.close();
}

int main(){
    ParseAll();
    return 0;
}