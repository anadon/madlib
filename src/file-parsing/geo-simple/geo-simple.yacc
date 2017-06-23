%{
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE* yyin;

void yyerror(const char* s);

static bool validFile = true;

//This is obnoxious and overgrown to account for the variability in the
//file specification.  This can be verified and put into a more sane 
//structure.
static std::unordered_map<std::string, std::vector<std::vector<std::string>>> parseHolder = std::unordered_map<std::string, std::vector<std::vector<std::string>>>();

%}

%union {
  long int ival;
  double fval;
  std::string sval;
}

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token <pval> N_KEY

%%

topLevelParseRule:
    KEY_IS_VALUE topLevelParseRule  {}
  | TABLE topLevelParseRule         {}
  | COMMENT topLevelParseRule       {}
  | KEY_IS_VALUE                    {}
  | TABLE                           {}
  | COMMENT                         {}
  ;

KEY_IS_VALUE:
    KEY_LAB_TYPE IS VALUE {
      if(!validFile) return;
      if(strcmp("commercial", $3) && strcmp("non-commercial", $3) && 
         strcmp("custom-commercial", $3) && strcmp("virtual", $3)){
        fprintf(stderr, "\"%s\" value is not in {commercial, "
            "non-commercial, custom-commercial, virtual}, but is %s\n", 
                                                                $1, $3);
        validFile = false;
        return;
      }
      
      if(!parseHolder.count($1)){
        parseHolder.emplace($1, std::vector<std::vector<std::string>>());
        parseHolder[$1][0].push_back(std::vector<std::string>());
        parseHolder[$1][0][0].push_back($3);
      }else{
        //TODO: error
      }
      
      
    }
    
  | KEY_TEST_ARCHETYPE IS VALUE {
      if(!validFile) return;
      if(strcmp("spotted DNA/cDNA", $3)
      && strcmp("spotted oligonucleotide", $3)
      && strcmp("in situ oligonucleotide", $3)
      && strcmp("antibody", $3)
      && strcmp("tissue", $3)
      && strcmp("SARST", $3)
      && strcmp("RT-PCR", $3)
      && strcmp("MPSS", $3)){
        fprintf(stderr, "\"%s\" value is not in in {spotted DNA/cDNA, "
          "spotted oligonucleotide, in situ oligonucleotide, antibody, "
                "tissue, SARST, RT-PCR, or MPSS}, but is %s\n", $1, $3);
        exit(EINVAL);
      }
      
      
      if(!parseHolder.count($1)){
        parseHolder.emplace($1, std::vector<std::string>());
        parseHolder.at($1).push_back($3);
      }else{
        //TODO: error
      }
    }
    
  | KEY_AUTHOR_NAMES IS VALUE {
      if(!validFile) return;
      
      
      
      if(!parseHolder.count($1)){
        parseHolder.emplace($1, std::vector<std::string>());
      }
      
      parseHolder.at($1).push_back($3);
      
    }
    
  | KEY_INDEXED_INF INTEGER IS VALUE {
      if(!validFile) return;
      
      if(!parseHolder.count($1)){
        parseHolder.emplace($1, std::vector<std::string>());
      }
      
      
      if(parseHolder.at($1).size() <= $2){
        parseHolder.at($1).resize($2+1);
      }
      
      parseHolder[$1][$2] = $3;
    }
    
  | KEY_INDEXED_1 INTEGER IS VALUE {
      if(!validFile) return;
      
    }
    
  | KEY_1_GSE IS VALUE {
      if(!validFile) return;
      
    }
    
  | KEY_1_GPL IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_1_GSM IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_INF_PMID IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_INF_FP IS VALUE {
      if(!validFile) return;
      FILE *check = fopen($3, "r");
      if(NULL == check){
        fprintf(stderr, "Warning: for key \"%s\", cannot confirm a "
                              "file at \"%s\" is accessible\n", $1, $3);
      }
      
    }
    
  | KEY_1_FP IS VALUE {
      if(!validFile) return;
      
    }
    
  | KEY_1_INT IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_INDEXED_INF_TAG_VAL INTEGER IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_1_120CHAR IS VALUE {
      if(!validFile) return;
      if($3.size() > 120){
        fprintf(stderr, "Key \"%s\" has a set value that is over 120 "
                                            "characters (%s)", $1, $3);
        validFile = false;
        return;
      } 
    }
    
  | KEY_1_255CHAR IS VALUE {
      if(!validFile) return;
      if($3.size() > 120){
        fprintf(stderr, "Key \"%s\" has a set value that is over 255 "
                                            "characters (%s)", $1, $3);
        validFile = false;
        return;
      } 
    
    }
    
  | KEY_INF_URL IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_INF_ASCII IS VALUE {
      if(!validFile) return;
    
    }
    
  | KEY_1_ASCII IS VALUE {
      if(!validFile) return;
    
    }
  ;


  /* NAME: KEY_LAB_TYPE
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, custom-commercial, virtual}
   ********************************************************************/
   //KEYS: Platform_distribution
KEY_LAB_TYPE:
    Platform_distribution {
      $$ = $1;
    }
  ;
  

  /* NAME: KEY_TEST_ARCHETYPE
   * Number per file   : 1
   * Value constraints : in {spotted DNA/cDNA, spotted oligonucleotide, in situ oligonucleotide, antibody, tissue, SARST, RT-PCR, or MPSS}
   ********************************************************************/
   //KEYS: Platform_technology
KEY_TEST_ARCHETYPE:
    Platform_technology {
    $$ = $1;
    }
  ;
  
  
  /* NAME: KEY_AUTHOR_NAMES
   * Number per file   : Infinity
   * Value constraints : In the form 
   * 'firstname,middleinitial,lastname'
   * 'firstname,lastname'
   * firstname must be at least one character and cannot contain spaces
   * middleinitial, if present, is one character
   * lastname is at least two characters and can contain spaces
   ********************************************************************/
   //KEYS: Platform_contributor, Series_contributor
KEY_AUTHOR_NAMES:
    Platform_contributor{
    $$ = $1;
    }
  | Series_contributor{
    $$ = $1;
    }
  ;
   
  /* NAME: KEY_INDEXED_INF
   * Number per file   : Infinity, indexed
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: Sample_organism_ch, Sample_treatment_protocol_ch, Sample_growth_protocol_ch
   //KEYS: Sample_extract_protocol_ch, Sample_label_protocol_ch, Series_variable
   //KEYS: Series_variable_description, Series_variable_sample_list
   //KEYS: Series_repeats, Series_repeats_sample_list, Sample_biomaterial_provider_ch
KEY_INDEXED_INF:
    Sample_organism_ch{
    $$ = $1;
    }
  | Sample_treatment_protocol_ch{
    $$ = $1;
    }
  | Sample_growth_protocol_ch{
    $$ = $1;
    }
  | Sample_extract_protocol_ch{
    $$ = $1;
    }
  | Sample_label_protocol_ch{
    $$ = $1;
    }
  | Series_variable{
    $$ = $1;
    }
  | Series_variable_description{
    $$ = $1;
    }
  | Series_variable_sample_list{
    $$ = $1;
    }
  | Series_repeats{
    $$ = $1;
    }
  | Series_repeats_sample_list{
    $$ = $1;
    }
  | Sample_biomaterial_provider_ch{
    $$ = $1;
    }
  ;


  /* NAME: KEY_INDEXED_1
   * Number per file   : 1, indexed
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: Sample_source_name_ch, Sample_molecule_ch, Sample_label_ch
KEY_INDEXED_1:
    Sample_source_name_ch{
    $$ = $1;
    }
  | Sample_molecule_ch{
    $$ = $1;
    }
  | Sample_label_ch{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_GSE
   * Number per file   : 1
   * Value constraints : A valid Series accession number (GSExxx)
   ********************************************************************/
   //KEYS: Series_geo_accession
KEY_1_GSE:
    Series_geo_accession{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_GPL
   * Number per file   : 1
   * Value constraints : ASCII, valid Platform accession number (GPLxxx)
   ********************************************************************/
   //KEYS: Platform_geo_accession, Sample_platform_id
KEY_1_GPL:
    Platform_geo_accession{
    $$ = $1;
    }
  | Sample_platform_id{
    $$ = $1;
    }
  ;



  /* NAME: KEY_1_GSM
   * Number per file   : 1
   * Value constraints : Integer, valid Sample accession number (GSMxxx)
   ********************************************************************/
   //KEYS: Sample_geo_accession, Series_sample_id
KEY_1_GSM:
    Sample_geo_accession{
    $$ = $1;
    }
  | Series_sample_id{
    $$ = $1;
    }
  ;


  /* NAME: KEY_INF_PMID
   * Number per file   : Infinity
   * Value constraints : ASCII, PubMed identifier
   ********************************************************************/
   //KEYS: Platform_pubmed_id, Series_pubmed_id
KEY_INF_PMID:
    Platform_pubmed_id{
    $$ = $1;
    }
  | Series_pubmed_id{
    $$ = $1;
    }
  ;


  /* NAME: KEY_INF_FP
   * Number per file   : Infinity
   * Value constraints : ASCII, Name of supplementary file, or 'none'.  
   ********************************************************************/
   //KEYS: Sample_supplementary_file
KEY_INF_FP:
    Sample_supplementary_file{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_FP
   * Number per file   : 1
   * Value constraints : ASCII, filepath, CHP or tab-delimited file
   ********************************************************************/
   //KEYS: Sample_table
KEY_1_FP:
    Sample_table{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_INT
   * Number per file   : 1
   * Value constraints : Interger
   ********************************************************************/
   //KEYS: Sample_tag_count, Sample_tag_length
KEY_1_INT:
    Sample_tag_count{
    $$ = $1;
    }
  | Sample_tag_length{
    $$ = $1;
    }
  ;


  /* NAME: KEY_INDEXED_INF_TAG_VAL
   * Number per file   : Infinity/channel
   * Value constraints : ASCII, 'Tag: Value' format
   ********************************************************************/
   //KEYS: Sample_characteristics_ch
KEY_INDEXED_INF_TAG_VAL:
    Sample_characteristics_ch{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_120CHAR
   * Number per file   : 1
   * Value constraints : ASCII, <= 120 characters
   ********************************************************************/
   //KEYS: Platform_title, Sample_title
KEY_1_120CHAR:
    Platform_title{
    $$ = $1;
    }
  | Sample_title{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_255CHAR
   * Number per file   : 1
   * Value constraints : ASCII, <= 255 characters
   ********************************************************************/
   //KEYS: Series_title
KEY_1_255CHAR:
    Series_title{
    $$ = $1;
    }
  ;


  /* NAME: KEY_INF_URL
   * Number per file   : Infinity
   * Value constraints : ASCII, valid URL
   ********************************************************************/
   //KEYS: Platform_web_link, Series_web_link
KEY_INF_URL:
    Platform_web_link{
    $$ = $1;
    }
  | Series_web_link{
    $$ = $1;
    }
  ;


  /* NAME: KEY_INF_ASCII
   * Number per file   : Infinity
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: Platform_organism, Platform_manufacture_protocol, Platform_catalog_number
   //KEYS: Platform_description, Sample_hyb_protocol, Sample_scan_protocol
   //KEYS: Sample_data_processing, Sample_description, Series_summary
KEY_INF_ASCII:
    Platform_organism{
    $$ = $1;
    }
  | Platform_manufacture_protocol{
    $$ = $1;
    }
  | Platform_catalog_number{
    $$ = $1;
    }
  | Platform_description{
    $$ = $1;
    }
  | Sample_hyb_protocol{
    $$ = $1;
    }
  | Sample_scan_protocol{
    $$ = $1;
    }
  | Sample_data_processing{
    $$ = $1;
    }
  | Sample_description{
    $$ = $1;
    }
  | Series_summary{
    $$ = $1;
    }
  ;


  /* NAME: KEY_1_ASCII
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: PLATFORM_TOKEN, Platform_manufacturer, Platform_support, Platform_coating
   //KEYS: SAMPLE_TOKEN, Sample_anchor, Sample_type, SERIES_TOKEN, Series_overall_design
KEY_1_ASCII:
    PLATFORM_TOKEN{
    $$ = $1;
    }
  | Platform_manufacturer{
    $$ = $1;
    }
  | Platform_support{
    $$ = $1;
    }
  | Platform_coating{
    $$ = $1;
    }
  | SAMPLE_TOKEN{
    $$ = $1;
    }
  | Sample_anchor{
    $$ = $1;
    }
  | Sample_type{
    $$ = $1;
    }
  | SERIES_TOKEN{
    $$ = $1;
    }
  | Series_overall_design{
    $$ = $1;
    }
  ;




TABLE:
  PLATFORM_TABLE_BEGIN
  PLATFORM_TABLE_END
  SAMPLE_TABLE_BEGIN
  SAMPLE_TABLE_END
  

  NUMBERED_KEY IS VALUE
  TABLE_START TABLE_DATA TABLE_END