#pragma once

class Command
{
public:
	Command() { }
	Command(Command && other) { }
	~Command() { }

	virtual void redo() = 0;
	virtual void undo() = 0;

private:
	
};