#pragma once

class Command
{
public:
	Command() { id = Command::command_count; }
	Command(Command && other) { }
	~Command() { }

	virtual void redo() = 0;
	virtual void undo() = 0;

	static void incrementCommandCounter() { command_count++; }
	static int command_count;
	int getID() { return id; }

protected:
	int id;
};