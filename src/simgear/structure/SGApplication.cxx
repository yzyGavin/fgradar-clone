/*
 * Copyright (C) 2012  Fernando García  <fernando.garli@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <simgear/debug/logstream.hxx>

#include "SGApplication.hxx"

SGSharedPtr<SGPropertyNode> SGApplication::m_property_tree = new SGPropertyNode;

/**
 * Allocate memory and initialize variables. argc and argv are given so the user
 * can use them or not, but it is not compulsory.
 */
SGApplication::SGApplication(int argc, char **argv) :
     m_quit_flag(false),
     m_subsystem_mgr(new SGSubsystemMgr)
{
     // Initializing the log should be the first thing we do, so other
     // subsystems can use it later
     sglog().setLogLevels(SG_ALL, SG_WARN);

     addCmdOption(std::string("--version"), onVersion);

     parseCmdOptions(argc, argv);
}

/**
 * Free previously allocated memory.
 */
SGApplication::~SGApplication()
{
}

/**
 * Run the main loop until 'm_quit_flag' is true.
 * This simple main loop updates all the subsystems (by updating the subsystem
 * manager). The update function actually needs the delta time, but FGRadar
 * doesn't need a lot of resources and it isn't a heavy 3D program, so passing 0
 * is a good solution.
 */
void
SGApplication::run()
{
     while (!m_quit_flag) {

          // Update subsystems
          m_subsystem_mgr->update(0.1);
     }
}

/**
 * Quit the program by setthing 'm_quit_flag' to true.
 * In the future, some things might be added, that's why it is recommended to
 * call this function instead of changing 'm_quit_flag' manually.
 */
void
SGApplication::quit()
{
     SG_LOG(SG_GENERAL, SG_INFO, "SGApplication quit signal");
     m_quit_flag = true;
}

void
SGApplication::parseCmdOptions(int argc, char **argv)
{
     for (int i = 1; i < argc; i++) {
          for(std::vector<CmdOption>::iterator j = m_cmd_options.begin();
              j != m_cmd_options.end(); j++) {
               
               if ((*j).cmd_name.compare(argv[i]) == 0) {
                    if (!(*j).function())
                         exit(0);
                    else
                         break;
               }
          }
     }
}

void
SGApplication::addCmdOption(std::string name, bool (*func)())
{
     CmdOption option;
     option.cmd_name = name;
     option.function = func;

     m_cmd_options.push_back(option);
}

bool
onVersion()
{
     std::cout << "I don't know!" << std::endl;

     return false;
}
