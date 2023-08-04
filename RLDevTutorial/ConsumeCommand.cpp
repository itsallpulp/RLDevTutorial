#include "ConsumeCommand.h"

ConsumeCommand::ConsumeCommand(Entity *consumer, Entity *consumed)
{
    this->consumed = consumed;
    this->consumer = consumer;
}

int ConsumeCommand::Execute()
{
    ConsumeItemEvent e(consumer, consumed);
    int cost = WorldFireEvent(&e);

    std::cout << cost << std::endl;
    
    if (consumer == player)
    {
        while (menus.size() > 0)
        {
            PopMenu();
        }
        gameState = ON_MAP;
    }

    return cost;
}
