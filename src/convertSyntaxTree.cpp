#include "./parser/Parser.h"
#include <map>


string createDefinition(Parser::Node* node, string nodeName, bool isRec = false) {
    if (node -> typeOfproc == "ASSIGN" || node -> typeOfproc == "READ") isRec = true;

    string changedLabel = "";
    if (node -> typeOfproc == "NUMBER") changedLabel = "const";
    else if (node -> typeOfproc == "IDENTIFIER") changedLabel = "id";
    else if (node -> typeOfproc == "EXP" || node -> typeOfproc == "SIMPLEEXP" || node -> typeOfproc == "TERM") changedLabel = "op";
    else changedLabel = node -> typeOfproc;

    string label = changedLabel + ((node -> valueAssociated != "") ? "\\n(" + node -> valueAssociated + ")" : "");
    nodeName += "[label=\"" + label + "\"";
    if(node -> typeOfproc == "EXP") nodeName += "   ordering=\"out\"";

    if (isRec) nodeName += "shape=rectangle";
    nodeName += "]";
    return nodeName;
}

string createPoint(Parser::Node* node, map<Parser::Node*, string>& nodeNames) {
    string point = nodeNames[node];
    point += " -> { " + nodeNames[node -> firstChild] + " ";
    if (node -> secondChild != nullptr) point += nodeNames[node -> secondChild];
    point += "}";
    return point;
}


void dfs (Parser::Node* node, map<Parser::Node*, string>& nodeNames, int& index, string& definition, vector<Parser::Node* >& sameLevel,
                vector<vector<Parser::Node*>>& childs, int level, bool isNext=false) {
    if (node == nullptr) return;
    index++;

    nodeNames[node] = ((node -> typeOfproc == "(EXP)") ? "EXP" : node -> typeOfproc) + to_string(index);
    definition += '\n' + createDefinition(node, nodeNames[node], isNext); 
    if (!isNext && node -> nextNode != nullptr) {
        // if (sameLevel.size() <= level) sameLevel.push_back(vector<string>());
        sameLevel.push_back(node);
    }
    if (node -> firstChild != nullptr || node -> secondChild != nullptr ) 
        childs.push_back({node, node -> firstChild, node -> secondChild});
    dfs(node -> firstChild, nodeNames, index, definition, sameLevel, childs, level + 1);
    dfs(node -> secondChild, nodeNames, index, definition, sameLevel, childs, level + 1);
    dfs(node -> nextNode, nodeNames, index, definition, sameLevel, childs, level, true);

}


string convertTree(Parser::Node* rootNode) {
    string dotContent = "";


    map<Parser::Node*, string> nodeNames;
    vector<vector<Parser::Node *>> childs;
    int index = 0;
    int level = 0;
    string definition = "";
    vector<Parser::Node*> sameLevel;
    dfs(rootNode -> nextNode, nodeNames, index, definition, sameLevel, childs, level, false);

    string arrows = "";
    for (int i = 0 ; i < childs.size(); ++i) {
        string nodeName = nodeNames[childs[i][0]];
        arrows += "\n" + createPoint(childs[i][0], nodeNames);
    }
    string sameRank = "";
    for (int i = 0; i < sameLevel.size(); ++i) {
        Parser::Node* temp = sameLevel[i];
        string level = nodeNames[temp];
        while(temp -> nextNode != nullptr) {
            temp = temp -> nextNode;
            level += " -> " + nodeNames[temp];
        }
        sameRank += "{rank=\"same\" \n" + level + "\n}\n";
    }
    dotContent = "digraph G {\n edge[arrowhead=none]\n" + definition + '\n' + sameRank + '\n' + arrows + '\n' + "}\n";
    return dotContent;
}
