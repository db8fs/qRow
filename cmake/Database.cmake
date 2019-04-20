
cmake_policy(SET CMP0053 OLD) # allow aliasing of CMake variables with set() command

set( Database_Directory_Root                        "${CMAKE_BINARY_DIR}/Database/"                         CACHE PATH "Root path to generated database stuff" )
set( Database_Schema_Directory_Root                 "${Database_Directory_Root}/Schema"                     CACHE PATH "Generated SQL script root directory" )
set( Database_Schema_Directory_ProcessVariables     "${Database_Schema_Directory_Root}/ProcessVariables"    CACHE PATH "Directory for generated SQL scripts of TimeSeries data" )

set( Database_Schema_Prefix_Entity              "ent_" 			CACHE STRING "String prefix for database entities (E/R)" )
set( Database_Schema_Prefix_Relationship        "rel_" 			CACHE STRING "String prefix for database relations (E/R)" )
set( Database_Schema_Prefix_Trigger             "trg_" 			CACHE STRING "String prefix for database triggers" )
set( Database_Schema_Prefix_View                "view_"			CACHE STRING "String prefix for database views" )
set( Database_Schema_Group_ProcessVariable	"Variable_"             CACHE STRING "String prefix for measurement variables (logged timeseries data)" )
set( Database_Schema_Group_Common               "Common_" 		CACHE STRING "String prefix for common tables (fundamental schema dependencies)" )

set( Database_Directory_Diagnostics 		"${Database_Directory_Root}/Diagnostics" CACHE STRING "Directory for DB schema diagnostics" )

set( Database_Diagnostics_Schema_EnableShow 	FALSE 	CACHE BOOL "" )


if( NOT EXISTS "${Database_Schema_Directory_Root}" )
	message( STATUS "Database: creating SQL schema inside '${Database_Schema_Directory_Root}'" )
	file( MAKE_DIRECTORY "${Database_Schema_Directory_Root}" )
endif()




# Pretty-prints a CMake list as a readable sql statement
#
# Outputs: variable 'sql' in parent scope

function( sqlQueryFormat statementList )
	STRING(REGEX REPLACE ";" "\n" statement "${ARGV}")
	set( sql "${statement};\n" PARENT_SCOPE )
endfunction()


# initializes the common tables to make the database schema work
function( initDatabase )
	configure_file( "${CMAKE_SOURCE_DIR}/Templates/Database/Common.sql.in" "${Database_Schema_Directory_Root}/Common.sql" )
endfunction()


# Returns the SQL CREATE statement for the entity
#
# group: 			entity group
# name: 			entity name
# attribute: 	entity attribute
#
# TODO: attribute handling with hardcoded INTEGER is crappy...

function( createTable_Entity result entityName entityPrimaryKey attribute ) 
	set( sql "" )
	sqlQueryFormat(     "CREATE TABLE @entityName@ ("
											"  @entityPrimaryKey@ INTEGER PRIMARY KEY AUTOINCREMENT,"
											"  @attribute@ INTEGER )" )
	
	set( "${result}" "${sql}"	 			PARENT_SCOPE)
endfunction()



# Returns the SQL CREATE statement for a relationship table between the two given entities
#
# OUT result:								created sql statement will be placed in this variable
# IN relationshipName:			name of the relationship
# IN relationshipKey:				name of the relationship's primary key
# IN srcEntityTable:				source entity's table name
# IN srcEntityPrimaryKey:		source entity's primary key
# IN destEntityTable:				dest entity's table name
# IN destEntityPrimaryKey:	dest entity's primary key

function( createTable_Relationship result relationshipName relationshipKey srcEntityTable srcEntityPrimaryKey destEntityTable destEntityPrimaryKey )
	set( sql "" )
	sqlQueryFormat(     "CREATE TABLE @relationshipName@ ("
											"  ${relationshipKey} INTEGER PRIMARY KEY AUTOINCREMENT,"
											"  sourceId INTEGER,"
											"  destId 	INTEGER,"
											"  FOREIGN KEY(sourceId) 	REFERENCES @srcEntityTable@ ( @srcEntityPrimaryKey@ ),"
											"  FOREIGN KEY(destId) 		REFERENCES @destEntityTable@( @destEntityPrimaryKey@ )"
											" )" )
	
	set( "${result}" 	"${sql}"	PARENT_SCOPE )
endfunction()


# Returns the SQL CREATE statement for a view on the given relationship
#
# OUT result:							created sql statement will be placed in this variable

function( createView result )

	set( viewName "view_Variable_AverageSpeed" )

	set( sql "" )
	sqlQueryFormat( 		"CREATE VIEW IF NOT EXISTS @viewName@ AS " 
											"  SELECT timestamp, msec, ent_Variable_AverageSpeed.speed "
											"    FROM ent_Timeseries "
											"      LEFT JOIN rel_Variable_AverageSpeed ON ( ent_Timeseries.id = rel_Variable_AverageSpeed.timeid )"
											"      LEFT JOIN ent_Variable_AverageSpeed ON ( ent_Variable_AverageSpeed.id = rel_Variable_AverageSpeed.AverageSpeedid )"
											"    WHERE ent_Variable_AverageSpeed.speed NOT NULL " 
											"    ORDER BY timestamp ASC, msec ASC"
	)
	
	set( "${result}" 	"${sql}"	PARENT_SCOPE )

endfunction()



# Adds the given process variable to the database schema
#
# IN: timestampTable
# IN: timestampPrimaryKey
# IN: group
# IN: measurementVariable
# IN: value

function( addProcessVariable timestampTable timestampPrimaryKey group measurementVariable value )
	
	# create an entity table holding the measurement values	
	set( entityName 						"@Database_Schema_Prefix_Entity@@group@@measurementVariable@"  )
	set( entityPrimaryKey 			"id" )
	set( sqlEntity 							" -- None!" )
	
	createTable_Entity( sqlEntity 	"${entityName}" "${entityPrimaryKey}" "${value}" )
	message( "${sqlEntity}\n" )
	
	
	# create an relationship table, linking the entity to the timestamp table
	set( relationshipName 			"@Database_Schema_Prefix_Relationship@@group@@measurementVariable@" )
	set( relationshipPrimaryKey "id" )
	set( sqlRelation						" -- None!" )
	
	createTable_Relationship( sqlRelation "${relationshipName}" "${relationshipPrimaryKey}" "${timestampTable}" "${timestampPrimaryKey}" "${entityName}" "${entityPrimaryKey}" )	
	message( "${sqlRelation}\n" )
	
	
	# create a view for simplyfing SQL statements
	set( sqlView )
	createView( sqlView "${viewName}" )
	message("${sqlView}\n") 
	
	
	if( measurementVariable STREQUAL "" )
		message( "Database: addProcessVariable ${measurementVariable} FAILED!" )	
	else()
		configure_file( "${CMAKE_SOURCE_DIR}/Templates/Database/ProcessVariable.sql.in" "${Database_Schema_Directory_ProcessVariables}/${measurementVariable}.sql"  )	
	endif()
	
endfunction()





# Creates the sql scripts for logging of process variables (aka timeseries data)
#
# 

function( setupMeasurements ) 

	if( NOT EXISTS "${Database_Schema_Directory_ProcessVariables}" )
		file( MAKE_DIRECTORY "${Database_Schema_Directory_ProcessVariables}" )
	endif()

	set( group 									"@Database_Schema_Group_ProcessVariable@" )
	set( timestampTable 				"@Database_Schema_Prefix_Entity@Timeseries" )
	set( timestampPrimaryKey 		"id" )

	initDatabase()
	addProcessVariable( "${timestampTable}" "${timestampPrimaryKey}" "${group}" "HeartRate" 		"bpm" )
	addProcessVariable( "${timestampTable}" "${timestampPrimaryKey}" "${group}" "AverageSpeed" 	"speed" )

endfunction()



# plots the diagram
#
#

function( plotDiagram sqliteDB )
    message( "Plotting Database: ${sqliteDB}" )

		# PlantUML template for E/R diagrams stolen from:
		# https://gist.github.com/QuantumGhost/0955a45383a0b6c0bc24f9654b3cb561
		
		
    set( entities "Table(group, \"group\\n(group of users)\") {\n primary_key(id) INTEGER\n not_null(name) VARCHAR[32] \n} " )
    #set( relationships "group \"1\" --> \"0..N\" group : \"A group may \ncontain many users\" ")

    configure_file( "${CMAKE_SOURCE_DIR}/templates/Database/Diagnosis/dbschema_template.uml.in" "${CMAKE_BINARY_DIR}/Database/Diagnostics/schema.uml" )

		if( Database_Diagnostics_Schema_EnableShow )
			if(WIN32)
	 			execute_process( COMMAND "cmd" "/c" "${Database_Tools_Display}" "${Database_Diagnostics_Schema_Diagram}" )
			else()
				execute_process( COMMAND "${Database_Tools_Display}" "${Database_Diagnostics_Schema_Diagram}" )
			endif()	 				
		endif()

endfunction()


# Sets up some database schema diagnostics tools
#
#

function( setupDiagnostics )

	if( NOT EXISTS "${Database_Directory_Diagnostics}" )
		file( MAKE_DIRECTORY "${Database_Directory_Diagnostics}" )
	endif()

	# ImageMagick's 'display' command differs for Win32 and Unix builds
	if( WIN32 )
		set( imdisplay "imdisplay")
	else()		
		set( imdisplay "display")
	endif()

	find_program( Database_Tools_SQLite     "sqlite3"       DOC "Path to the SQLite3 commandline binary\n\nCMake can use that to create a database from the schema" )
	find_program( Database_Tools_PlantUML   "plantuml.jar" 	DOC "Path to the PlantUML Java Archive\n\nCMake can use that to draw the E/R diagram of the schema" )
	find_program( Database_Tools_Display    "${imdisplay}" 	DOC "Path to the ImageMagick display binary\n\nCMake can use that for drawing the rendered E/R diagram of the schema" )


	if( NOT EXISTS "${Database_Tools_SQLite}" )
		message( STATUS "Database: SQLite Binary not found - skipping DB schema validation" )
	else()
		set( Database_Diagnostics_Schema_SQLite  		"${Database_Directory_Diagnostics}/schema.db"		CACHE STRING "SQLite database created from the generated .sql schema scripts" )
		#createDatabase( "${Database_Schema_Diagnostics_SQLite}" )
	endif()


	if( NOT EXISTS "${Database_Tools_PlantUML}" )
		message( STATUS "Database: PlantUML Binary not found - skipping DB schema plot" )
	else()
		set( Database_Diagnostics_Schema_Diagram		"${Database_Directory_Diagnostics}/schema.png"	CACHE STRING "E/R diagram of the database schema" )
		
		plotDiagram( "${Database_Schema_Diagnostics_SQLite}" )
	endif()

endfunction()



#############

setupMeasurements()
setupDiagnostics()

