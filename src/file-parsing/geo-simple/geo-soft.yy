%defines
%define parser_class_name {GeoSoftDriver}
%define api.token.constructor
%define parse.assert
%locations
%start topLevelParseRule
%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}
%require "3.0.4"
%glr-parser
%skeleton "lalr1.cc"
//%define api.pure full
//%define parser.error verbose
%locations
%token-table


%initial-action{
  @$.begin.filename = @$.end.filename = &driver.filename;
  //TODO: this is clearly wrong, as Driver only cares about a FILE*.  The @$
  //thing needs to also use FILE*.
}

%code top {
  #include <deque>
  #include <stdio.h>
  #include <string>
  #include <string.h>
  #include <vector>
  #include <unordered_map>
%}


%code provides{

#include "geo-soft.tab.hh"

void yyparse(yyscan_t *scanner);
void yylex(YYSTYPE *yylval_param, yyscan_t yyscanner);


%}



//the value used as "$X" for all the parse rules below
//bison builds the union automatically this way in a way usefil for C++
%define api.value.type variant
%token <std::string> VALUE GSE_NUMBER GPL_NUMBER GSM_NUMBER URL
%token <std::string> PLATFORM_TOKEN PLATFORM_TITLE PLATFORM_DISTRIBUTION
%token <std::string> PLATFORM_TECHNOLOGY PLATFORM_ORGANISM
%token <std::string> PLATFORM_MANUFACTURER PLATFORM_MANUFACTURE_PROTOCOL
%token <std::string> PLATFORM_CATALOG_NUMBER PLATFORM_WEB_LINK
%token <std::string> PLATFORM_SUPPORT PLATFORM_COATING
%token <std::string> PLATFORM_DESCRIPTION PLATFORM_CONTRIBUTOR
%token <std::string> PLATFORM_PUBMED_ID PLATFORM_GEO_ACCESSION
%token <std::string> PLATFORM_TABLE_BEGIN PLATFORM_TABLE_END
%token <std::string> SAMPLE_TOKEN SAMPLE_TITLE SAMPLE_SUPPLEMENTARY_FILE
%token <std::string> SAMPLE_TABLE SAMPLE_SOURCE_NAME_CH
%token <std::string> SAMPLE_ORGANISM_CH SAMPLE_CHARACTERISTICS_CH
%token <std::string> SAMPLE_BIOMATERIAL_PROVIDER_CH
%token <std::string> SAMPLE_TREATMENT_PROTOCOL_CH
%token <std::string> SAMPLE_GROWTH_PROTOCOL_CH SAMPLE_MOLECULE_CH
%token <std::string> SAMPLE_EXTRACT_PROTOCOL_CH SAMPLE_LABEL_CH
%token <std::string> SAMPLE_LABEL_PROTOCOL_CH SAMPLE_HYB_PROTOCOL
%token <std::string> SAMPLE_SCAN_PROTOCOL SAMPLE_DATA_PROCESSING
%token <std::string> SAMPLE_DESCRIPTION SAMPLE_PLATFORM_ID
%token <std::string> SAMPLE_GEO_ACCESSION SAMPLE_ANCHOR SAMPLE_TYPE
%token <std::string> SAMPLE_TAG_COUNT SAMPLE_TAG_LENGTH
%token <std::string> SAMPLE_TABLE_BEGIN SAMPLE_TABLE_END SERIES_TOKEN
%token <std::string> SERIES_TITLE SERIES_SUMMARY SERIES_OVERALL_DESIGN
%token <std::string> SERIES_PUBMED_ID SERIES_WEB_LINK SERIES_CONTRIBUTOR
%token <std::string> SERIES_VARIABLE SERIES_VARIABLE_DESCRIPTION
%token <std::string> SERIES_VARIABLE_SAMPLE_LIST SERIES_REPEATS
%token <std::string> SERIES_REPEATS_SAMPLE_LIST SERIES_SAMPLE_ID
%token <std::string> SERIES_GEO_ACCESSION RESEARCHER_NAME TAG_VALUE
%token <int> INTEGER
%token IS LINE_END WHITESPACE COMMENT
%type <std::deque<std::vector<std::string> > > tabledata
%type < std::vector<std::string> > researcher_names tag_value_list tablerow
%type <std::string> key_lab_type key_test_archetype key_author_names
%type <std::string> key_indexed_inf key_indexed_1 key_1_gse key_1_gpl
%type <std::string> key_1_gsm key_inf_pmid key_inf_fp key_1_fp key_1_int
%type <std::string> key_indexed_inf_tag_val key_1_120char key_1_255char
%type <std::string> key_inf_url key_inf_ascii key_1_ascii


%code{
//yylex and yyerror need to be declared here
FILE* yyin;
void yyerror(YYLTYPE *locp, const char* s);

class Scanner;
class Interpreter;

static Parser::symbol_type vyylex(Scanner &scanner, Interpreter &interpreter){
  return scanner.get_next_token();
}

%}


%%

topLevelParseRule:
    key_is_value LINE_END topLevelParseRule {}
  | table        LINE_END topLevelParseRule {}
  | COMMENT      LINE_END topLevelParseRule {}
  | LINE_END                                {}
  ;

key_is_value:
    key_lab_type IS VALUE {
      if(strcmp("commercial", $3) && strcmp("non-commercial", $3) &&
         strcmp("custom-commercial", $3) && strcmp("virtual", $3)){
        fprintf(stderr, "ERROR: line %d characters %d - %d, key value "
            "\"%s\" is not in "
            "{commercial, non-commercial, custom-commercial, virtual}, "
                          "but is %s\n", @3.first_line, @3.first_column,
                                                @3.last_column, $1, $3);
        validFile = false;
        return;
      }

      ensureSpaceForKey($1);
      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);
    }

  | key_test_archetype IS VALUE {

      if(strcmp("spotted DNA/cDNA", $3)
      && strcmp("spotted oligonucleotide", $3)
      && strcmp("in situ oligonucleotide", $3)
      && strcmp("antibody", $3)
      && strcmp("tissue", $3)
      && strcmp("SARST", $3)
      && strcmp("RT-PCR", $3)
      && strcmp("MPSS", $3)){
        fprintf(stderr, "ERROR: line %d characters %d - %d, key value "
              "\"%s\" is not in in {spotted DNA/cDNA, "
              "spotted oligonucleotide, in situ oligonucleotide, "
              "antibody, tissue, SARST, RT-PCR, or MPSS}, but is %s\n",
                @3.first_line, @3.first_column, @3.last_column, $1, $3);
        validFile = false;
      }


      ensureSpaceForKey($1);
      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;
      driver.parseHolder[$1][0].push_back($3);

    }

  | key_author_names IS researcher_names {

      ensureSpaceForKey($1);

      for(size_t i = 0; i < $3.size(); i++)
        driver.parseHolder[$1][0].push_back($3[i]);

    }

  | key_indexed_inf INTEGER IS VALUE {

      ensureSpaceForKey($1);

      while(driver.parseHolder[$1].size() <= $2)
        driver.parseHolder[$1].push_back(std::vector<std::string>());

      driver.parseHolder[$1][$2].push_back($4);
    }

  | key_indexed_1 INTEGER IS VALUE {

      ensureSpaceForKey($1);

      while(driver.parseHolder[$1].size() <= $2)
        driver.parseHolder[$1].push_back(std::vector<std::string>());

      if(!isUniqueInsertForChannel($1, $4, $2, @4.first_line,
                              @4.first_column, @4.last_column)) return;

      driver.parseHolder[$1][$2].push_back($4);
    }

  | key_1_gse IS GSE_NUMBER {

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);
    }

  | key_1_gpl IS GPL_NUMBER {

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_1_gsm IS GSM_NUMBER {

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_inf_pmid IS INTEGER {

      ensureSpaceForKey($1);

      driver.parseHolder[$1][0].push_back($3);
    }

  | key_inf_fp IS VALUE {

      FILE *check = fopen($3, "r");
      if(NULL == check){
        fprintf(stderr, "Warning: line %d characters %d - %d, key "
                    "value \"%s\", cannot confirm a file at \"%s\" is "
                    "accessible\n", @3.first_line, @3.first_column,
                                                @3.last_column, $1, $3);
      }
      fclose(check);

      ensureSpaceForKey($1, $3, @3.first_line, @3.first_column,
                                                        @3.last_column);

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_1_fp IS VALUE {

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1)) return;

      FILE *check = fopen($3, "r");
      if(NULL == check){
        fprintf(stderr, "Warning: line %d characters %d - %d, key "
                    "value \"%s\", cannot confirm a file at \"%s\" is "
                        "accessible\n", @3.first_line, @3.first_column,
                                                @3.last_column, $1, $3);
      }
      fclose(check);

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_1_int IS INTEGER {

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_indexed_inf_tag_val INTEGER IS tag_value_list {

      ensureSpaceForKey($1);

      while(driver.parseHolder[$1].size() <= $2)
        driver.parseHolder[$1].push_back(std::vector<std::string>());

      for(size_t i = 0; i < $4.size(); i++)
        driver.parseHolder[$1][$2].push_back($4[i]);

    }

  | key_1_120char IS VALUE {

      if($3.size() > 120){
        fprintf(stderr, "Error: line %d characters %d - %d, key value "
              "\"%s\" has a set value that is over 120 characters (%s)",
                @3.first_line, @3.first_column, @3.last_column, $1, $3);
        validFile = false;
      }

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);
    }

  | key_1_255char IS VALUE {

      if($3.size() > 255){
        fprintf(stderr, "Error: line %d characters %d - %d, key value "
              "\"%s\" has a set value that is over 255 characters (%s)",
                @3.first_line, @3.first_column, @3.last_column, $1, $3);
        validFile = false;
      }

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_inf_url IS URL {

      ensureSpaceForKey($1);

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_inf_ascii IS VALUE {

      ensureSpaceForKey($1);

      driver.parseHolder[$1][0].push_back($3);

    }

  | key_1_ascii IS VALUE {

      ensureSpaceForKey($1);

      if(!isUniqueInsert($1, $3, @3.first_line, @3.first_column,
                                                @3.last_column)) return;

      driver.parseHolder[$1][0].push_back($3);

    }
  ;


  /* NAME: key_lab_type
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, custom-commercial, virtual}
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
   //KEYS: Sample_organism_ch, Sample_treatment_protocol_ch, Sample_growth_protocol_ch
   //KEYS: Sample_extract_protocol_ch, Sample_label_protocol_ch, Series_variable
   //KEYS: Series_variable_description, Series_variable_sample_list
   //KEYS: Series_repeats, Series_repeats_sample_list, Sample_biomaterial_provider_ch
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
   //KEYS: Platform_organism, Platform_manufacture_protocol, Platform_catalog_number
   //KEYS: Platform_description, Sample_hyb_protocol, Sample_scan_protocol
   //KEYS: Sample_data_processing, Sample_description, Series_summary
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
   //KEYS: PLATFORM_TOKEN, Platform_manufacturer, Platform_support, Platform_coating
   //KEYS: SAMPLE_TOKEN, Sample_anchor, Sample_type, SERIES_TOKEN, Series_overall_design
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
      std::vector<std::string> nameList;
      nameList.push_back($1);
      $$ = nameList;
    }
  | researcher_names WHITESPACE RESEARCHER_NAME {
      $3.push_back($1);
      $$ = $3;
    }
  ;


//NOTE: WHITE_SPACE may be problematic due to ambiguity
tag_value_list:
    TAG_VALUE{
      std::vector<std::string> tr;
      tr.push_back($1);
      $$ = tr;
    }
  | tag_value_list WHITESPACE TAG_VALUE{
      $3.push_back($1);
      $$ = $3;
    }
  ;


table:
  PLATFORM_TABLE_BEGIN LINE_END tabledata PLATFORM_TABLE_END
  SAMPLE_TABLE_BEGIN LINE_END tabledata SAMPLE_TABLE_END


tabledata:
    tablerow tabledata {
      $1.push_front($2);
      $$ = $1;
    }
  | tablerow {
      $$ = std::deque<std::vector<std::string>>();
      $$.push_back($1);
    }
  ;

tablerow:
    LINE_END {
      $$ = std::vector<std::string>();
    }
  | VALUE WHITESPACE tablerow {
      $3.push_back($1);
      $$ = $3;
    }


%%

//epilogue

//yylex and yyerror need to be defined here

int yylex(){

}


void yy::Parser::error(const location_type &l, const string &m){
  driver.error(l, m);
}


int parseSoftFile(const char *path, struct GEOSoftFile contents){
  int status = 0;

  status = freopen(stdin, path);
  if(status){
    return status;
  }

  std::unordered_map<std::string, std::vector<std::vector<std::string> > > driver();

  yyscan_t myscanner;

  yylex_init(&myscanner);
  yyparse(myscanner, &driver);
  yylex_destroy(myscanner);

  if(status){
    return status;
  }

  return status;
}
