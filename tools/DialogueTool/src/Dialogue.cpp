#include "Dialogue.h"
#include "Nodes/StartNode.h"
#include "Nodes/ChoiceNode.h"
#include "Nodes/NPCNode.h"
#include "FileIO/ConfigurationIO.h"
#include "ApplicationState.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

Dialogue::Dialogue(const std::string& npcID) {
	m_npcID = npcID;
	m_startNode = new StartNode();
}

Dialogue::~Dialogue() {
	delete m_startNode;
}

StartNode* Dialogue::getStartNode() const {
	return m_startNode;
}

const std::string&  Dialogue::getNpcID() const {
	return m_npcID;
}

int Dialogue::generateTag() {
	int tag = 1;
	while (m_usedTags.find(tag) != m_usedTags.end()) {
		++tag;
	}
	m_usedTags.insert(tag);
	return tag;
}

void Dialogue::freeTag(int tag) {
	if (tag < 1) return;
	auto pos = m_usedTags.find(tag);
	if (pos == m_usedTags.end()) return;
	m_usedTags.erase(pos);
}

bool Dialogue::exportDialogue() {
	// first create a directory
	std::string folderPath = G_CONF.npcFolder + "/" + m_npcID;
	int nError = 0;
#if defined(_WIN32)
	nError = _mkdir(folderPath.c_str()); // windows style mkdir
#else 
	nError = mkdir(sPath.c_str(), 0733); // non-Windows style mkdir
#endif
	if (nError != 0) {
		struct stat info;
		if (stat(folderPath.c_str(), &info) != 0) {
			ERROR("[Dialogue]: Unable to create directory: " + folderPath);
			return false;
		}
	}

	// then create the file  (lua export)
	std::string luaPath = folderPath + "/dl_" + m_npcID + ".lua";
	std::ofstream dialogueFile(luaPath, std::ios::trunc);
	if (dialogueFile.is_open()) {
		dialogueFile << exportToLua();
		dialogueFile.close();
	}
	else {
		ERROR("[Dialogue]: Lua export: Unable to open file: " + luaPath);
		return false;
	}

	// then the sql export
	std::string sqlPath = G_CONF.sqlFolder + "/insert_text_dl_" + m_npcID + "_utf8.sql";
	std::ofstream sqlFile(sqlPath, std::ios::trunc);
	if (sqlFile.is_open()) {
		sqlFile << exportToSQL();
		sqlFile.close();
	}
	else {
		ERROR("[Dialogue]: SQL export: Unable to open file: " + sqlPath);
		return false;
	}

	return true;
}

void Dialogue::recursiveLuaExport(DialogueNode* node, std::string& stack, int indentationLevel) const {
	if (node == nullptr) return;

	stack.append(node->exportToLua(indentationLevel) + "\n");

	for (auto child : node->getLinkNodes()) {
		recursiveLuaExport(child->nextNode, stack, indentationLevel);
	}
}

std::string Dialogue::exportToLua() const {
	std::stringstream ss;
	int indentationLevel = 0;
	ss << "-- Dialogue for NPC \"" << m_npcID << "\"\n";

	ss << "loadDialogue = function(DL) \n\n";

	std::string stack = "";
	recursiveLuaExport(m_startNode, stack, indentationLevel + 1);
	ss << stack;

	ss << "end";
	return ss.str();
}

void Dialogue::recursiveSQLExport(DialogueNode* node, std::string& stack) const {
	if (node == nullptr) return;

	stack.append(node->exportToSQL());

	for (auto child : node->getLinkNodes()) {
		recursiveSQLExport(child->nextNode, stack);
	}
}

std::string Dialogue::exportToSQL() const {
	std::string stack = "";
	recursiveSQLExport(m_startNode, stack);
	return stack;
}