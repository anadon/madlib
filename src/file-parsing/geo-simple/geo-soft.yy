%defines
//%define parse.assert
%locations
%start topLevelParseRule
%require "3.0.4"
%define api.pure full
%locations
%token-table


//%initial-action{
//  @$.begin.filename = @$.end.filename = &driver.filename;
//  //TODO: this is clearly wrong, as Driver only cares about a FILE*.  The @$
//  //thing needs to also use FILE*.
//%}

%code top {
  #include <deque>
  #include <stdio.h>
  #include <string>
  #include <string.h>
  #include <vector>
  #include <unordered_map>

  using namespace std;


}

%code requires{

  #include <deque>
  #include <stdio.h>
  #include <string>
  #include <string.h>
  #include <vector>
  #include <unordered_map>

  using namespace std;

}


%union{
  vector<string> *vs;
  int local_int;
  deque<vector<string> > *dvs;
  string *s;
};


%code provides{

  #include <deque>
  #include <stdio.h>
  #include <string>
  #include <string.h>
  #include <vector>
  #include <unordered_map>
  #include <cstdlib>

  using namespace std;

  #include "geo-soft.tab.hh"
  #include "lex.yy.h"

  //These are gross hacks.
  static bool validFile = true;
  static std::unordered_map<std::string, std::vector<std::vector<std::string> > > *intermediate;

  void ensureSpaceForKey(
                  unordered_map<string, vector<vector<string> > > &intermediate,
                                                                    string key){
    if(0 == intermediate.count(key)){
      intermediate[key] = vector<vector<string>>();
      intermediate[key].push_back(vector<string>());
    }
  }

  bool isUniqueInsert(
                  unordered_map<string, vector<vector<string> > > &intermediate,
                    string key, string value, int first_line, int first_column,
                                                              int last_column){
    if(0 == intermediate.count(key)) return true;
    fprintf(stderr, "Error: repeat insertion of key \"%s\" with value \"%s\" "
                    "on line %d from columns %d-%d\n", key.c_str(), first_line,
                                                    first_column, last_column);
    return false;
  }

}



//the value used as "$X" for all the parse rules below
//bison builds the union automatically this way in a way useful for C++
//%define api.value.type variant

%token <s> VALUE GSE_NUMBER GPL_NUMBER GSM_NUMBER URL
%token <s> PLATFORM_TOKEN PLATFORM_TITLE PLATFORM_DISTRIBUTION
%token <s> PLATFORM_TECHNOLOGY PLATFORM_ORGANISM
%token <s> PLATFORM_MANUFACTURER PLATFORM_MANUFACTURE_PROTOCOL
%token <s> PLATFORM_CATALOG_NUMBER PLATFORM_WEB_LINK
%token <s> PLATFORM_SUPPORT PLATFORM_COATING
%token <s> PLATFORM_DESCRIPTION PLATFORM_CONTRIBUTOR
%token <s> PLATFORM_PUBMED_ID PLATFORM_GEO_ACCESSION
%token <s> PLATFORM_TABLE_BEGIN PLATFORM_TABLE_END
%token <s> SAMPLE_TOKEN SAMPLE_TITLE SAMPLE_SUPPLEMENTARY_FILE
%token <s> SAMPLE_TABLE SAMPLE_SOURCE_NAME_CH
%token <s> SAMPLE_ORGANISM_CH SAMPLE_CHARACTERISTICS_CH
%token <s> SAMPLE_BIOMATERIAL_PROVIDER_CH
%token <s> SAMPLE_TREATMENT_PROTOCOL_CH
%token <s> SAMPLE_GROWTH_PROTOCOL_CH SAMPLE_MOLECULE_CH
%token <s> SAMPLE_EXTRACT_PROTOCOL_CH SAMPLE_LABEL_CH
%token <s> SAMPLE_LABEL_PROTOCOL_CH SAMPLE_HYB_PROTOCOL
%token <s> SAMPLE_SCAN_PROTOCOL SAMPLE_DATA_PROCESSING
%token <s> SAMPLE_DESCRIPTION SAMPLE_PLATFORM_ID
%token <s> SAMPLE_GEO_ACCESSION SAMPLE_ANCHOR SAMPLE_TYPE
%token <s> SAMPLE_TAG_COUNT SAMPLE_TAG_LENGTH
%token <s> SAMPLE_TABLE_BEGIN SAMPLE_TABLE_END SERIES_TOKEN
%token <s> SERIES_TITLE SERIES_SUMMARY SERIES_OVERALL_DESIGN
%token <s> SERIES_PUBMED_ID SERIES_WEB_LINK SERIES_CONTRIBUTOR
%token <s> SERIES_VARIABLE SERIES_VARIABLE_DESCRIPTION
%token <s> SERIES_VARIABLE_SAMPLE_LIST SERIES_REPEATS
%token <s> SERIES_REPEATS_SAMPLE_LIST SERIES_SAMPLE_ID
%token <s> SERIES_GEO_ACCESSION RESEARCHER_NAME TAG_VALUE
%token <s> INTEGER
%token IS LINE_END WHITESPACE COMMENT
%type <dvs> tabledata
%type <vs> researcher_names tag_value_list tablerow
%type <s> key_lab_type key_test_archetype key_author_names
%type <s> key_indexed_inf key_indexed_1 key_1_gse key_1_gpl
%type <s> key_1_gsm key_inf_pmid key_inf_fp key_1_fp key_1_int
%type <s> key_indexed_inf_tag_val key_1_120char key_1_255char
%type <s> key_inf_url key_inf_ascii key_1_ascii

%%

topLevelParseRule:
    key_is_value LINE_END topLevelParseRule {}
  | table        LINE_END topLevelParseRule {}
  | COMMENT      LINE_END topLevelParseRule {}
  | LINE_END                                {}
  ;

key_is_value:
    key_lab_type IS VALUE {
      string key = *$1;
      string value = *$3;
      if(strcmp("commercial", value.c_str()) &&
         strcmp("non-commercial", value.c_str()) &&
         strcmp("custom-commercial", value.c_str()) &&
         strcmp("virtual", value.c_str())){
        fprintf(stderr, "ERROR: line %d characters %d - %d, key value "
            "\"%s\" is not in "
            "{commercial, non-commercial, custom-commercial, virtual}, "
                          "but is %s\n", @3.first_line, @3.first_column,
                                      @3.last_column, key.c_str(), value.c_str());
        validFile = false;
      }

      ensureSpaceForKey(*intermediate, key);
      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);
    }

  | key_test_archetype IS VALUE {
    string key = *$1;
    string value = *$3;

      if(strcmp("spotted DNA/cDNA", value.c_str())
      && strcmp("spotted oligonucleotide", value.c_str())
      && strcmp("in situ oligonucleotide", value.c_str())
      && strcmp("antibody", value.c_str())
      && strcmp("tissue", value.c_str())
      && strcmp("SARST", value.c_str())
      && strcmp("RT-PCR", value.c_str())
      && strcmp("MPSS", value.c_str())){
        fprintf(stderr, "ERROR: line %d characters %d - %d, key value "
              "\"%s\" is not in in {spotted DNA/cDNA, "
              "spotted oligonucleotide, in situ oligonucleotide, "
              "antibody, tissue, SARST, RT-PCR, or MPSS}, but is %s\n",
                @3.first_line, @3.first_column, @3.last_column, key.c_str(),
                                                                  value.c_str());
        validFile = false;
      }


      ensureSpaceForKey(*intermediate, key);
      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                      @3.last_column)) return 1;
      intermediate->at(key).at(0).push_back(value);

    }

  | key_author_names IS researcher_names {
      string key = *$1;

      ensureSpaceForKey(*intermediate, key);

      for(size_t i = 0; i < $3->size(); i++)
        intermediate->at(key).at(0).push_back((*$3)[i]);

    }

  | key_indexed_inf INTEGER IS VALUE {
    string key = *$1;
    int index = $2;
    string value = *$4;

      ensureSpaceForKey(*intermediate, key);

      while(intermediate->at(key).size() <= $2)
        intermediate->at(key).push_back(std::vector<string>());

      intermediate->at(key).at($2).push_back(*$4);
    }

  | key_indexed_1 INTEGER IS VALUE {
    string key = *$1;
    int index = $2;
    string value = *$4;

      ensureSpaceForKey(*intermediate, key);

      while(intermediate->at(key).size() <= $2)
        intermediate->at(key).push_back(std::vector<string>());

      if(!isUniqueInsertForChannel(key, *$4, $2, @4.first_line,
                              @4.first_column, @4.last_column)) return 1;

      intermediate->at(key).at($2).push_back(*$4);
    }

  | key_1_gse IS GSE_NUMBER {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);
    }

  | key_1_gpl IS GPL_NUMBER {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);

    }

  | key_1_gsm IS GSM_NUMBER {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);

    }

  | key_inf_pmid IS INTEGER {
    string key = *$1;
    string value = to_string($3);

      ensureSpaceForKey(*intermediate, key);

      intermediate->at(key).at(0).push_back(value);
    }

  | key_inf_fp IS VALUE {
    string key = *$1;
    string value = *$3;

      FILE *check = fopen($3->c_str(), "r");
      if(NULL == check){
        fprintf(stderr, "Warning: line %d characters %d - %d, key "
                    "value \"%s\", cannot confirm a file at \"%s\" is "
                    "accessible\n", @3.first_line, @3.first_column,
                                      @3.last_column, $1->c_str(), $3->c_str());
      }
      fclose(check);

      ensureSpaceForKey(*intermediate, key);

      intermediate->at(key).at(0).push_back(value);

    }

  | key_1_fp IS VALUE {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                    @3.last_column)) return 1;

      FILE *check = fopen($3->c_str(), "r");
      if(NULL == check){
        fprintf(stderr, "Warning: line %d characters %d - %d, key "
                    "value \"%s\", cannot confirm a file at \"%s\" is "
                        "accessible\n", @3.first_line, @3.first_column,
                                      @3.last_column, $1->c_str(), $3->c_str());
      }
      fclose(check);

      intermediate->at(key).at(0).push_back(value);

    }

  | key_1_int IS INTEGER {
    string key = *$1;
    string value = to_string($3);

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);

    }

  | key_indexed_inf_tag_val INTEGER IS tag_value_list {
    string key = *$1;
    int index = $2;
    vector<string> values = *$4;

      ensureSpaceForKey(*intermediate, key);

      while(intermediate->at(key).size() <= index)
        intermediate->at(key).push_back(std::vector<string>());

      for(size_t i = 0; i < $4->size(); i++)
        intermediate->at(key).at(index).push_back(values[i]);

    }

  | key_1_120char IS VALUE {
    string key = *$1;
    string value = *$3;

      if($3->size() > 120){
        fprintf(stderr, "Error: line %d characters %d - %d, key value "
                  "\"%s\" has a set value that is over 120 characters (%s)",
                  @3.first_line, @3.first_column, @3.last_column, $1->c_str(),
                                                                  $3->c_str());
        validFile = false;
      }

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);
    }

  | key_1_255char IS VALUE {
    string key = *$1;
    string value = *$3;

      if($3->size() > 255){
        fprintf(stderr, "Error: line %d characters %d - %d, key value "
              "\"%s\" has a set value that is over 255 characters (%s)",
                    @3.first_line, @3.first_column, @3.last_column, $1->c_str(),
                                                                  $3->c_str());
        validFile = false;
      }

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);

    }

  | key_inf_url IS URL {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      intermediate->at(key).at(0).push_back(value);

    }

  | key_inf_ascii IS VALUE {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      intermediate->at(key).at(0).push_back(value);

    }

  | key_1_ascii IS VALUE {
    string key = *$1;
    string value = *$3;

      ensureSpaceForKey(*intermediate, key);

      if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
                                                @3.last_column)) return 1;

      intermediate->at(key).at(0).push_back(value);

    }
  ;


  /* NAME: key_lab_type
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, custom-commercial,
   * virtual}
   ********************************************************************/
   //KEYS: PLATFORM_DISTRIBUTION
key_lab_type:
    PLATFORM_DISTRIBUTION {
      $$ = $1;
    }
  ;


  /* NAME: key_test_archetype
   * Number per file   : 1
   * Value constraints : in {spotted DNA/cDNA, spotted oligonucleotide,
   *  in situ oligonucleotide, antibody, tissue, SARST, RT-PCR, or MPSS}
   ********************************************************************/
   //KEYS: PLATFORM_TECHNOLOGY
key_test_archetype:
    PLATFORM_TECHNOLOGY {
      $$ = $1;
    }
  ;


  /* NAME: key_author_names
   * Number per file   : Infinity
   * Value constraints : In the form
   * 'firstname,middleinitial,lastname'
   * 'firstname,lastname'
   * firstname must be at least one character and cannot contain spaces
   * middleinitial, if present, is one character
   * lastname is at least two characters and can contain spaces
   ********************************************************************/
   //KEYS: PLATFORM_CONTRIBUTOR, Series_contributor
key_author_names:
    PLATFORM_CONTRIBUTOR{
      $$ = $1;
    }
  | SERIES_CONTRIBUTOR{
      $$ = $1;
    }
  ;

  /* NAME: key_indexed_inf
   * Number per file   : Infinity, indexed
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: Sample_organism_ch, Sample_treatment_protocol_ch,
   //KEYS: Sample_growth_protocol_ch, Sample_extract_protocol_ch,
   //KEYS: Sample_label_protocol_ch, Series_variable,
   //KEYS: Series_variable_description, Series_variable_sample_list,
   //KEYS: Series_repeats, Series_repeats_sample_list,
   //KEYS: Sample_biomaterial_provider_ch
key_indexed_inf:
    SAMPLE_ORGANISM_CH{
      $$ = $1;
    }
  | SAMPLE_TREATMENT_PROTOCOL_CH{
      $$ = $1;
    }
  | SAMPLE_GROWTH_PROTOCOL_CH{
      $$ = $1;
    }
  | SAMPLE_EXTRACT_PROTOCOL_CH{
      $$ = $1;
    }
  | SAMPLE_LABEL_PROTOCOL_CH{
      $$ = $1;
    }
  | SERIES_VARIABLE{
      $$ = $1;
    }
  | SERIES_VARIABLE_DESCRIPTION{
      $$ = $1;
    }
  | SERIES_VARIABLE_SAMPLE_LIST{
      $$ = $1;
    }
  | SERIES_REPEATS{
      $$ = $1;
    }
  | SERIES_REPEATS_SAMPLE_LIST{
      $$ = $1;
    }
  | SAMPLE_BIOMATERIAL_PROVIDER_CH{
      $$ = $1;
    }
  ;


  /* NAME: key_indexed_1
   * Number per file   : 1, indexed
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: Sample_source_name_ch, Sample_molecule_ch, Sample_label_ch
key_indexed_1:
    SAMPLE_SOURCE_NAME_CH{
      $$ = $1;
    }
  | SAMPLE_MOLECULE_CH{
      $$ = $1;
    }
  | SAMPLE_LABEL_CH{
      $$ = $1;
    }
  ;


  /* NAME: key_1_gse
   * Number per file   : 1
   * Value constraints : A valid Series accession number (GSExxx)
   ********************************************************************/
   //KEYS: Series_geo_accession
key_1_gse:
    SERIES_GEO_ACCESSION{
      $$ = $1;
    }
  ;


  /* NAME: key_1_gpl
   * Number per file   : 1
   * Value constraints : ASCII, valid Platform accession number (GPLxxx)
   ********************************************************************/
   //KEYS: Platform_geo_accession, Sample_platform_id
key_1_gpl:
    PLATFORM_GEO_ACCESSION{
      $$ = $1;
    }
  | SAMPLE_PLATFORM_ID{
      $$ = $1;
    }
  ;



  /* NAME: key_1_gsm
   * Number per file   : 1
   * Value constraints : Integer, valid Sample accession number (GSMxxx)
   ********************************************************************/
   //KEYS: Sample_geo_accession, Series_sample_id
key_1_gsm:
    SAMPLE_GEO_ACCESSION{
      $$ = $1;
    }
  | SERIES_SAMPLE_ID{
      $$ = $1;
    }
  ;


  /* NAME: key_inf_pmid
   * Number per file   : Infinity
   * Value constraints : ASCII, PubMed identifier
   ********************************************************************/
   //KEYS: Platform_pubmed_id, Series_pubmed_id
key_inf_pmid:
    PLATFORM_PUBMED_ID{
      $$ = $1;
    }
  | SERIES_PUBMED_ID{
      $$ = $1;
    }
  ;


  /* NAME: key_inf_fp
   * Number per file   : Infinity
   * Value constraints : ASCII, Name of supplementary file, or 'none'.
   ********************************************************************/
   //KEYS: Sample_supplementary_file
key_inf_fp:
    SAMPLE_SUPPLEMENTARY_FILE{
      $$ = $1;
    }
  ;


  /* NAME: key_1_fp
   * Number per file   : 1
   * Value constraints : ASCII, filepath, CHP or tab-delimited file
   ********************************************************************/
   //KEYS: Sample_table
key_1_fp:
    SAMPLE_TABLE{
      $$ = $1;
    }
  ;


  /* NAME: key_1_int
   * Number per file   : 1
   * Value constraints : Interger
   ********************************************************************/
   //KEYS: Sample_tag_count, Sample_tag_length
key_1_int:
    SAMPLE_TAG_COUNT{
      $$ = $1;
    }
  | SAMPLE_TAG_LENGTH{
      $$ = $1;
    }
  ;


  /* NAME: key_indexed_inf_tag_val
   * Number per file   : Infinity/channel
   * Value constraints : ASCII, 'Tag: Value' format
   ********************************************************************/
   //KEYS: Sample_characteristics_ch
key_indexed_inf_tag_val:
    SAMPLE_CHARACTERISTICS_CH{
      $$ = $1;
    }
  ;


  /* NAME: key_1_120char
   * Number per file   : 1
   * Value constraints : ASCII, <= 120 characters
   ********************************************************************/
   //KEYS: Platform_title, Sample_title
key_1_120char:
    PLATFORM_TITLE{
      $$ = $1;
    }
  | SAMPLE_TITLE{
      $$ = $1;
    }
  ;


  /* NAME: key_1_255char
   * Number per file   : 1
   * Value constraints : ASCII, <= 255 characters
   ********************************************************************/
   //KEYS: Series_title
key_1_255char:
    SERIES_TITLE{
      $$ = $1;
    }
  ;


  /* NAME: key_inf_url
   * Number per file   : Infinity
   * Value constraints : ASCII, valid URL
   ********************************************************************/
   //KEYS: Platform_web_link, Series_web_link
key_inf_url:
    PLATFORM_WEB_LINK{
      $$ = $1;
    }
  | SERIES_WEB_LINK{
      $$ = $1;
    }
  ;


  /* NAME: key_inf_ascii
   * Number per file   : Infinity
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: Platform_organism, Platform_manufacture_protocol,
   //KEYS: Platform_catalog_number, Platform_description, Sample_hyb_protocol,
   //KEYS: Sample_scan_protocol, Sample_data_processing, Sample_description,
   //KEYS: Series_summary
key_inf_ascii:
    PLATFORM_ORGANISM{
      $$ = $1;
    }
  | PLATFORM_MANUFACTURE_PROTOCOL{
      $$ = $1;
    }
  | PLATFORM_CATALOG_NUMBER{
      $$ = $1;
    }
  | PLATFORM_DESCRIPTION{
      $$ = $1;
    }
  | SAMPLE_HYB_PROTOCOL{
      $$ = $1;
    }
  | SAMPLE_SCAN_PROTOCOL{
      $$ = $1;
    }
  | SAMPLE_DATA_PROCESSING{
      $$ = $1;
    }
  | SAMPLE_DESCRIPTION{
      $$ = $1;
    }
  | SERIES_SUMMARY{
      $$ = $1;
    }
  ;


  /* NAME: key_1_ascii
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: PLATFORM_TOKEN, Platform_manufacturer, Platform_support,
   //KEYS: Platform_coating, SAMPLE_TOKEN, Sample_anchor, Sample_type,
   //KEYS: SERIES_TOKEN, Series_overall_design
key_1_ascii:
    PLATFORM_TOKEN{
      $$ = $1;
    }
  | PLATFORM_MANUFACTURER{
      $$ = $1;
    }
  | PLATFORM_SUPPORT{
      $$ = $1;
    }
  | PLATFORM_COATING{
      $$ = $1;
    }
  | SAMPLE_TOKEN{
      $$ = $1;
    }
  | SAMPLE_ANCHOR{
      $$ = $1;
    }
  | SAMPLE_TYPE{
      $$ = $1;
    }
  | SERIES_TOKEN{
      $$ = $1;
    }
  | SERIES_OVERALL_DESIGN{
      $$ = $1;
    }
  ;


researcher_names:
    RESEARCHER_NAME{
      string value = *$1;
      vector<string> *nameList;
      nameList = new vector<string>();
      nameList->push_back(value);
      $$ = nameList;
    }
  | researcher_names WHITESPACE RESEARCHER_NAME {
      string value = *$3;
      $1->push_back(value);
      $$ = $1;
    }
  ;


//NOTE: WHITE_SPACE may be problematic due to ambiguity
tag_value_list:
    TAG_VALUE{
      string value = *$1;
      vector<string> *tr = new vector<string>();
      tr->push_back(value);
      $$ = tr;
    }
  | tag_value_list WHITESPACE TAG_VALUE{
      string value = *$3;
      $1->push_back(value);
      $$ = $1;
    }
  ;


table:
  PLATFORM_TABLE_BEGIN LINE_END tabledata PLATFORM_TABLE_END
  SAMPLE_TABLE_BEGIN LINE_END tabledata SAMPLE_TABLE_END


tabledata:
    tablerow tabledata {
      $2->push_front(*$1);
      $$ = $2;
    }
  | tablerow {
      $$ = new deque<vector<string>>();
      $$->push_front(*$1);
    }
  ;

tablerow:
    LINE_END {
      $$ = new vector<string>();
    }
  | VALUE WHITESPACE tablerow {
      string value = *$1;
      $3->push_back(value);
      $$ = $3;
    }


%%

//epilogue

//yylex and yyerror need to be defined here



//void yy::Parser::error(const location_type &l, const string &m){
//  driver.error(l, m);
//}




int loadGeoSoftFile(const char *fp, struct GeoSoft **contents){
  int status = 0;

  status = freopen(stdin, path);
  if(status){
    return status;
  }

  //unordered_map<string, vector<vector<string> > > *intermediate;
  intermediate = new unordered_map<string, vector<vector<string> > >();


  yyparse();

  if(status){
    return status;
  }







  CSTRING_Sample_supplementary_file
  CSTRING_Sample_table
  CSTRING_Sample_source_name_ch
  CSTRING_Sample_organism_ch
  CSTRING_Sample_characteristics_ch
  CSTRING_Sample_biomaterial_provider_ch
  CSTRING_Sample_treatment_protocol_ch
  CSTRING_Sample_growth_protocol_ch
  CSTRING_Sample_molecule_ch
  CSTRING_Sample_extract_protocol_ch
  CSTRING_Sample_label_ch
  CSTRING_Sample_label_protocol_ch
  CSTRING_Sample_hyb_protocol
  CSTRING_Sample_scan_protocol
  CSTRING_Sample_data_processing
  CSTRING_Sample_description
  CSTRING_Sample_platform_id
  CSTRING_Sample_geo_accession
  CSTRING_Sample_anchor
  CSTRING_Sample_type
  CSTRING_Sample_tag_count
  CSTRING_Sample_tag_length
  CSTRING_Sample_table_begin
  CSTRING_Sample_table_end
  CSTRING_SERIES
  CSTRING_Series_title
  CSTRING_Series_summary
  CSTRING_Series_overall_design
  CSTRING_Series_pubmed_id
  CSTRING_Series_web_link
  CSTRING_Series_contributor
  CSTRING_Series_variable_
  CSTRING_Series_variable_description_
  CSTRING_Series_variable_sample_list_
  CSTRING_Series_repeats_
  CSTRING_Series_repeats_sample_list_
  CSTRING_Series_sample_id
  CSTRING_Series_geo_accession


  if(!intermediate->count(CSTRING_PLATFORM)){
    (*contents)->PLATFORM_TOKEN
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_title)){
    (*contents)->PLATFORM_TITLE
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_distribution)){
    (*contents)->PLATFORM_DISTRIBUTION
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_technology)){
    (*contents)->PLATFORM_TECHNOLOGY
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_organism)){
    (*contents)->PLATFORM_ORGANISM
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_manufacturer)){
    (*contents)->PLATFORM_MANUFACTURER
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_manufacture_protocol)){
    (*contents)->PLATFORM_MANUFACTURE_PROTOCOL
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_catalog_number)){
    (*contents)->PLATFORM_CATALOG_NUMBER
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_web_link)){
    (*contents)->PLATFORM_WEB_LINK
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_support)){
    (*contents)->PLATFORM_SUPPORT
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_coating)){
    (*contents)->PLATFORM_COATING
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_description)){
    (*contents)->PLATFORM_DESCRIPTION
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_contributor)){
    (*contents)->PLATFORM_CONTRIBUTOR
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_pubmed_id)){
    (*contents)->PLATFORM_PUBMED_ID
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_geo_accession)){
    (*contents)->PLATFORM_GEO_ACCESSION
  }else{
  }


  if(!intermediate->count(CSTRING_Platform_table_begin)){
    (*contents)->PLATFORM_TABLE_BEGIN
  }else{
  }


  //should be handled by the CSTRING_Platform_table_begin clause
  //if(!intermediate->count(CSTRING_Platform_table_end)){
  //  (*contents)->PLATFORM_TABLE_END
  //}else{
  //}


  if(!intermediate->count(CSTRING_SAMPLE)){
    (*contents)->
  }else{
  }


  if(!intermediate->count(CSTRING_Sample_title)){
    (*contents)->SAMPLE_TITLE
  }else{
  }


  if(!intermediate->count("!Sample_supplementary_file")){
    (*contents)->SAMPLE_SUPPLEMENTARY_FILE
  }else{
  }


  if(!intermediate->count("!Sample_table")){
    (*contents)->SAMPLE_TABLE
  }else{
  }


  if(!intermediate->count("!Sample_source_name_ch")){
    (*contents)->SAMPLE_SOURCE_NAME_CH
  }else{
  }


  if(!intermediate->count("!Sample_organism_ch")){
    (*contents)->SAMPLE_ORGANISM_CH
  }else{
  }


  if(!intermediate->count("!Sample_characteristics_ch")){
    (*contents)->SAMPLE_CHARACTERISTICS_CH
  }else{
  }


  if(!intermediate->count("!Sample_biomaterial_provider_ch")){
    (*contents)->SAMPLE_BIOMATERIAL_PROVIDER_CH
  }else{
  }


  if(!intermediate->count("!Sample_treatment_protocol_ch")){
    (*contents)->SAMPLE_TREATMENT_PROTOCOL_CH
  }else{
  }


  if(!intermediate->count("!Sample_growth_protocol_ch")){
    (*contents)->SAMPLE_GROWTH_PROTOCOL_CH
  }else{
  }


  if(!intermediate->count("!Sample_molecule_ch")){
    (*contents)->SAMPLE_MOLECULE_CH
  }else{
  }


  if(!intermediate->count("!Sample_extract_protocol_ch")){
    (*contents)->SAMPLE_EXTRACT_PROTOCOL_CH
  }else{
  }


  if(!intermediate->count("!Sample_label_ch")){
    (*contents)->SAMPLE_LABEL_CH
  }else{
  }


  if(!intermediate->count("!Sample_label_protocol_ch")){
    (*contents)->SAMPLE_LABEL_PROTOCOL_CH
  }else{
  }


  if(!intermediate->count("!Sample_hyb_protocol")){
    (*contents)->SAMPLE_HYB_PROTOCOL
  }else{
  }


  if(!intermediate->count("!Sample_scan_protocol")){
    (*contents)->SAMPLE_SCAN_PROTOCOL
  }else{
  }


  if(!intermediate->count("!Sample_data_processing")){
    (*contents)->SAMPLE_DATA_PROCESSING
  }else{
  }


  if(!intermediate->count("!Sample_description")){
    (*contents)->SAMPLE_DESCRIPTION
  }else{
  }


  if(!intermediate->count("!Sample_platform_id")){
    (*contents)->SAMPLE_PLATFORM_ID
  }else{
  }


  if(!intermediate->count("!Sample_geo_accession")){
    (*contents)->SAMPLE_GEO_ACCESSION
  }else{
  }


  if(!intermediate->count("!Sample_anchor")){
    (*contents)->SAMPLE_ANCHOR
  }else{
  }


  if(!intermediate->count("!Sample_type")){
    (*contents)->SAMPLE_TYPE
  }else{
  }


  if(!intermediate->count("!Sample_tag_count")){
    (*contents)->SAMPLE_TAG_COUNT
  }else{
  }


  if(!intermediate->count("!Sample_tag_length")){
    (*contents)->SAMPLE_TAG_LENGTH
  }else{
  }


  if(!intermediate->count("!Sample_table_begin")){
    (*contents)->SAMPLE_TABLE_BEGIN
  }else{
  }


  if(!intermediate->count("!Sample_table_end")){
    (*contents)->SAMPLE_TABLE_END
  }else{
  }


  if(!intermediate->count("[^]SERIES")){
    (*contents)->SERIES_TOKEN
  }else{
  }

  if(!intermediate->count("!Series_title")){
    (*contents)->SERIES_TITLE
  }else{
  }


  if(!intermediate->count("!Series_summary")){
    (*contents)->SERIES_SUMMARY
  }else{
  }


  if(!intermediate->count("!Series_overall_design")){
    (*contents)->SERIES_OVERALL_DESIGN
  }else{
  }


  if(!intermediate->count("!Series_pubmed_id")){
    (*contents)->SERIES_PUBMED_ID
  }else{
  }


  if(!intermediate->count("!Series_web_link")){
    (*contents)->SERIES_WEB_LINK
  }else{
  }


  if(!intermediate->count("!Series_contributor")){
    (*contents)->SERIES_CONTRIBUTOR
  }else{
  }


  if(!intermediate->count("!Series_variable_")){
    (*contents)->SERIES_VARIABLE
  }else{
  }


  if(!intermediate->count("!Series_variable_description_")){
    (*contents)->SERIES_VARIABLE_DESCRIPTION
  }else{
  }


  if(!intermediate->count("!Series_variable_sample_list_")){
    (*contents)->SERIES_VARIABLE_SAMPLE_LIST
  }else{
  }


  if(!intermediate->count("!Series_repeats_")){
    (*contents)->SERIES_REPEATS
  }else{
  }


  if(!intermediate->count("!Series_repeats_sample_list_")){
    (*contents)->SERIES_REPEATS_SAMPLE_LIST
  }else{
  }


  if(!intermediate->count("!Series_sample_id")){
    (*contents)->SERIES_SAMPLE_ID
  }else{
  }


  if(!intermediate->count("!Series_geo_accession")){
    (*contents)->SERIES_GEO_ACCESSION
  }else{
  }

    std::string GSE_NUMBER
    std::string GPL_NUMBER
    std::string GSM_NUMBER
    std::string URL
    std::string PLATFORM_TOKEN
    std::string PLATFORM_TITLE
    std::string PLATFORM_DISTRIBUTION
    std::string PLATFORM_TECHNOLOGY
    std::string PLATFORM_ORGANISM
    std::string PLATFORM_MANUFACTURER
    std::string PLATFORM_MANUFACTURE_PROTOCOL
    std::string PLATFORM_CATALOG_NUMBER
    std::string PLATFORM_WEB_LINK
    std::string PLATFORM_SUPPORT
    std::string PLATFORM_COATING
    std::string PLATFORM_DESCRIPTION
    std::string PLATFORM_CONTRIBUTOR
    std::string PLATFORM_PUBMED_ID
    std::string PLATFORM_GEO_ACCESSION
    std::string PLATFORM_TABLE_BEGIN
    std::string PLATFORM_TABLE_END
    std::string SAMPLE_TOKEN
    std::string SAMPLE_TITLE
    std::string SAMPLE_SUPPLEMENTARY_FILE
    std::string SAMPLE_TABLE
    std::string SAMPLE_SOURCE_NAME_CH
    std::string SAMPLE_ORGANISM_CH
    std::string SAMPLE_CHARACTERISTICS_CH
    std::string SAMPLE_BIOMATERIAL_PROVIDER_CH
    std::string SAMPLE_TREATMENT_PROTOCOL_CH
    std::string SAMPLE_GROWTH_PROTOCOL_CH
    std::string SAMPLE_MOLECULE_CH
    std::string SAMPLE_EXTRACT_PROTOCOL_CH
    std::string SAMPLE_LABEL_CH
    std::string SAMPLE_LABEL_PROTOCOL_CH
    std::string SAMPLE_HYB_PROTOCOL
    std::string SAMPLE_SCAN_PROTOCOL
    std::string SAMPLE_DATA_PROCESSING
    std::string SAMPLE_DESCRIPTION
    std::string SAMPLE_PLATFORM_ID
    std::string SAMPLE_GEO_ACCESSION
    std::string SAMPLE_ANCHOR SAMPLE_TYPE
    std::string SAMPLE_TAG_COUNT
    std::string SAMPLE_TAG_LENGTH
    std::string SAMPLE_TABLE_BEGIN
    std::string SAMPLE_TABLE_END SERIES_TOKEN
    std::string SERIES_TITLE SERIES_SUMMARY
    std::string SERIES_OVERALL_DESIGN
    std::string SERIES_PUBMED_ID
    std::string SERIES_WEB_LINK
    std::string SERIES_CONTRIBUTOR
    std::string SERIES_VARIABLE
    std::string SERIES_VARIABLE_DESCRIPTION
    std::string SERIES_VARIABLE_SAMPLE_LIST
    std::string SERIES_REPEATS
    std::string SERIES_REPEATS_SAMPLE_LIST
    std::string SERIES_SAMPLE_ID
    std::string SERIES_GEO_ACCESSION
    std::string RESEARCHER_NAME
    std::string TAG_VALUE


  return status;
}
