#ifndef PARSER_H_
# define PARSER_H_

#include <QSharedPointer>

class Parser
{
public:
	Parser( class IProtocolHandler & protocolHandler );
	~Parser();

	bool parse(const QString & line);
	
private:
	struct Waterrower_Grammar* const	m_grammar;
};


#endif // PARSER_H_