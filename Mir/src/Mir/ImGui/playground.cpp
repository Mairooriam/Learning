#include "playground.h"

#include "Types/MirTypes.h"

#include "TypeUtils/Serializers.h"
#include "TypeUtils/Sorting.h"

#include "b&rParser/Tokenizer.h"
#include "b&rParser/MirParser.h"
#include "b&rParser/Mapper.h"
#include "b&rParser/BaseWidgets.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Mir {
    void playground(){
        ImGui::Begin("Playground");

        Types::StructDefinition test9090;
        static bool mapperinitialized = false;
        static bool intializedtable = false; 
        if (ImGui::Button("Maper tihngy"))
        {         
            intializedtable = false;
            mapperinitialized = false;
            json configtest;     
            std::ifstream f = Utils::File::openFile("C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\src\\Mir\\b&rParser\\csvMapping.json");
            if (f.is_open()) {
                configtest = json::parse(f);
            }
            

            // Parse CSV data
            Types::CSV::Settings settings;
            settings.delimeter = ',';
            settings.targetFile = "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\Luotu.csv";
            Parser::CsvParser parser(settings);
            Types::CSV::Data csvData = parser.parse();
            Mapper mapper(configtest,csvData);

            test9090 = mapper.process();

            mapperinitialized = true;
        }
        static UI::StructTable testtable;
        
        if (mapperinitialized)
        {
            if (!intializedtable)
            {
                testtable.InitializeFromStructDef(test9090);
                intializedtable = true;
            } else {
                testtable.Render();
            }
            
            
        }


        ImGui::End();
    }

}