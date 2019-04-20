

# generates a QVector of sample logfiles
function( generateTest_Logfiles_QVector path)

    file( GLOB Data_Logfiles_All "${path}")
    list( LENGTH Data_Logfiles_All numberLogfiles )

    set( path ${CMAKE_CURRENT_SOURCE_DIR} )

    foreach( item ${Data_Logfiles_All} )
        set( vector "${vector}\n    vector.append( \"${item}\" )\\;" )
    endforeach()

    set( Data_Logfiles_QVector  ${vector}           PARENT_SCOPE )
    set( Data_Logfiles_Number   ${numberLogfiles}   PARENT_SCOPE )
    set( Data_Logfiles_Path     ${path}             PARENT_SCOPE )
endfunction()





