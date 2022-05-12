#include "engine_options.hpp"

namespace sm
{
    EngineOptions::EngineOptions()
    {
        m_engineOptions = std::unordered_map<std::string, EngineOption>();
        /*
           the value in MB for memory for hash tables can be changed,
           this should be answered with the first "setoptions" command at program boot
           if the engine has sent the appropriate "option name Hash" command,
           which should be supported by all engines!
           So the engine should use a very small hash first as default.
       */
        m_engineOptions["Hash"] = {true, "", "1 min 1 max 128", "spin"};

        /*
            this is the path on the hard disk to the Nalimov compressed format.
            Multiple directories can be concatenated with ";"
        */
        m_engineOptions["NalimovPath"] = {false, "", "true", "string"};

        /*
            this is the size in MB for the cache for the nalimov table bases
            These last two options should also be present in the initial options exchange dialog
        */
        m_engineOptions["NalimovCache"] = {false, "", "1 min 1 max 32", "spin"};

        /*
            this means that the engine is able to ponder.
            The GUI will send this whenever pondering is possible or not.
            Note: The engine should not start pondering on its own if this is enabled, this option is only
            needed because the engine might change its time management algorithm when pondering is allowed.
        */
        m_engineOptions["Ponder"] = {true, "", "", "check"};

        /*
            this means that the engine has its own book which is accessed by the engine itself.
            if this is set, the engine takes care of the opening book and the GUI will never
            execute a move out of its book for the engine. If this is set to false by the GUI,
            the engine should not access its own book.
        */
        m_engineOptions["OwnBook"] = {false, "", "", "check"};

        /*
            the engine supports multi best line or k-best mode. the default value is 1
        */
        m_engineOptions["MultiPV"] = {false, "", "1", "spin"};

        /*
            the engine can show the current line it is calculating. see "info currline" above.
        */
        m_engineOptions["UCI_ShowCurrLine"] = {false, "", "false", "check"};

        /*
            the engine can show a move and its refutation in a line. see "info refutations" above.
        */
        m_engineOptions["UCI_ShowRefutations"] = {false, "", "false", "check"};

        /*
            The engine is able to limit its strength to a specific Elo number,
            This should always be implemented together with "UCI_Elo".
        */
        m_engineOptions["UCI_LimitStrength"] = {false, "", "false", "check"};

        /*
            The engine can limit its strength in Elo within this interval.
            If UCI_LimitStrength is set to false, this value should be ignored.
            If UCI_LimitStrength is set to true, the engine should play with this specific strength.
            This should always be implemented together with "UCI_LimitStrength".
        */
        m_engineOptions["UCI_Elo"] = {false, "", "", "spin"};

        /*
            The engine wants to behave differently when analysing or playing a game.
            For example when playing it can use some kind of learning.
            This is set to false if the engine is playing a game, otherwise it is true.
        */
        m_engineOptions["UCI_AnalyseMode"] = {false, "", "", "check"};

        /*
            With this command the GUI can send the name, title, elo and if the engine is playing a human or computer to the engine.
            The format of the string has to be [GM|IM|FM|WGM|WIM|none] [\<elo\>|none] [computer|human] \<name\>
            Examples:
                "setoption name UCI_Opponent value GM 2800 human Gary Kasparov"
                "setoption name UCI_Opponent value none none computer Shredder"
        */
        m_engineOptions["UCI_Opponent"] = {false, "", "", "string"};

        /*
            With this command, the engine tells the GUI information about itself, for example a license text, usually it doesn't make sense that the GUI changes this text with the setoption command.
            Example:
                "option name UCI_EngineAbout type string default Shredder by Stefan Meyer-Kahlen, see www.shredderchess.com"
        */
        m_engineOptions["UCI_EngineAbout"] = {true, "", "SchwachMatt from the students of TH-OWL", "string"};

        /*
            this is either the path to the folder on the hard disk containing the Shredder endgame databases or the path and filename of one Shredder endgame datbase.
        */
        m_engineOptions["UCI_ShredderbasesPath"] = {false, "", "", "string"};

        /*
            the GUI can send this to the engine to tell the engine to use a certain value in centipawns from white's point of view if evaluating this specifix position.
            The string can have the formats:
                \<value\> + \<fen\> | clear + \<fen\> | clearall
        */
        m_engineOptions["UCI_SetPositionValue"] = {false, "", "", "string"};

        /*
            the Engine supports the nullmove or not
        */
        m_engineOptions["Nullmove"] = {true, "", "true", "check"};

        /*
            the playstyle of the engine (aggressive or passive etc)
        */
        m_engineOptions["Style"] = {false, "", "Normal var Solid var Normal var Risky", "combo"};
    }
    
    EngineOptions::~EngineOptions()
    {
        
    }

    bool EngineOptions::isSupported(const std::string &id) const
    {
        return (m_engineOptions.find(id) != m_engineOptions.end());
    }

    void EngineOptions::setValue(const std::string &id, const std::string &value)
    {
        if (isSupported(id))
        {
            m_engineOptions[id].value = value;
        }
        else
        {
            throw("InvalidOptionException: The option \"" + id + "\" is not defined in the UCI spec.");
        }
    }

    const std::string &EngineOptions::getValue(const std::string &id) const
    {
        if (isSupported(id))
        {
            return m_engineOptions.at(id).value;
        }
        else
        {
            throw("InvalidOptionException: The option \"" + id + "\" is not defined in the UCI spec.");
        }
    }
    
    const EngineOption& EngineOptions::getOption(const std::string& id) const
    {
        if (isSupported(id))
        {
            return m_engineOptions.at(id);
        }
        else
        {
            throw("InvalidOptionException: The option \"" + id + "\" is not defined in the UCI spec.");
        }
    }
    
    const std::unordered_map<std::string, EngineOption>& EngineOptions::getEngineOptions() const
    {
        return m_engineOptions;
    }

}