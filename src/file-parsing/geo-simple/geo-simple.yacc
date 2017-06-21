%{
#include <stdio.h>
#include <string>
#include <string.h>

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE* yyin;

void yyerror(const char* s);
%}

%union {
  long int ival;
  double fval;
  std::string sval;
  std::pair<std::string, int> pval;
}

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token <pval> N_KEY

%%

topLevelParseRule:
    KEY_IS_VALUE topLevelParseRule  {}
  | TABLE topLevelParseRule         {}
  | KEY_IS_VALUE                    {}
  | TABLE                           {}
  ;

KEY_IS_VALUE:
    SKEY IS VALUE {} //string value
  | NKEY IS VALUE {} //number value
  | WKEY IS VALUE {} //is a URL
  | FKEY IS VALUE {} //is a filepath
  | KEY_120CHARS IS VALUE {} // limited to 120 characters
  | KEY_255CHARS IS VALUE {} // limited to 255 characters
  | KEY_PMID IS VALUE {} //is a Pubmed ID
  | KEY_CHANNEL IS VALUE {} //is a key+channel
  | KEY_NAME IS VALUE {} //Is a list of author names
  | IGNORE_KEY {}
  ;


  /* NAME: KEY_LAB_TYPE
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, custom-commercial}
   ********************************************************************/
   //KEYS: 

  /* NAME: KEY_TEST_ARCHETYPE
   * Number per file   : 1
   * Value constraints : in {spotted DNA/cDNA, spotted oligonucleotide, in situ oligonucleotide, antibody, tissue, SARST, RT-PCR, or MPSS}
   ********************************************************************/
   //KEYS: 
  
  /* NAME: KEY_AUTHOR_NAMES
   * Number per file   : Infinity
   * Value constraints : In the form 
   * 'firstname,middleinitial,lastname'
   * 'firstname,lastname'
   * firstname must be at least one character and cannot contain spaces
   * middleinitial, if present, is one character
   * lastname is at least two characters and can contain spaces
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_TAB_LIST
   * Label/Key         : !Sample_data_processing
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Provide details of how data in the VALUE column of your 
   * table were generated and calculated, i.e., normalization method, 
   * data selection procedures and parameters, transformation algorithm 
   * (e.g., MAS5.0), and scaling parameters. You can include as much 
   * text as you need to thoroughly describe the processing procedures.
   ********************************************************************/ 
   //KEYS: 
   
  /* NAME: KEY_INF_INDEXED
   * Number per file   : Infinity, indexed
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: 
  
  /* NAME: KEY_INF_GSE
   * Number per file   : Infinity
   * Value constraints : A valid Series accession number (GSExxx)
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_1_GSE
   * Number per file   : 1
   * Value constraints : A valid Series accession number (GSExxx)
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_1_GPL
   * Number per file   : 1
   * Value constraints : ASCII, valid Platform accession number (GPLxxx)
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_1_GSM
   * Number per file   : 1
   * Value constraints : Integer, valid Sample accession number (GSMxxx)
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_INF_PMID
   * Number per file   : Infinity
   * Value constraints : ASCII, PubMed identifier
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_INF_FP
   * Number per file   : Infinity
   * Value constraints : ASCII, Name of supplementary file, or 'none'.  
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_1_FP
   * Number per file   : 1
   * Value constraints : ASCII, filepath, CHP or tab-delimited file
   ********************************************************************/
   //KEYS: 
  
  /* NAME: KEY_1_INT
   * Number per file   : 1
   * Value constraints : Interger
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_INF_INDEXED_TAG_VAL
   * Number per file   : 1 per channel
   * Value constraints : ASCII, 'Tag: Value' format
   ********************************************************************/
   //KEYS: 
  
  /* NAME: KEY_1_120CHAR
   * Number per file   : 1
   * Value constraints : ASCII, <= 120 characters
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_1_255CHAR
   * Number per file   : 1
   * Value constraints : ASCII, <= 255 characters
   ********************************************************************/
   //KEYS: 
  
  /* NAME: KEY_INF_URL
   * Number per file   : Infinity
   * Value constraints : ASCII, valid URL
   ********************************************************************/
   //KEYS: 
   
  /* NAME: KEY_INF_ASCII
   * Number per file   : Infinity
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: 
  
  /* NAME: KEY_1_ASCII
   * Number per file   : 1
   * Value constraints : ASCII
   ********************************************************************/
   //KEYS: 



SKEY:
  PLATFORM_TOKEN
  PLATFORM_TITLE
  PLATFORM_DISTRIBUTION
  PLATFORM_TECHNOLOGY
  PLATFORM_ORGANISM
  PLATFORM_MANUFACTURER
  PLATFORM_MANUFACTURE_PROTOCOL
  PLATFORM_CATALOG_NUMBER
  PLATFORM_WEB_LINK
  PLATFORM_SUPPORT
  PLATFORM_COATING
  PLATFORM_DESCRIPTION
  PLATFORM_CONTRIBUTOR
  PLATFORM_PUBMED_ID
  PLATFORM_GEO_ACCESSION
  SAMPLE_TITLE
  SAMPLE_SOURCE_NAME_CH
  SAMPLE_ORGANISM_CH
  SAMPLE_CHARACTERISTICS_CH
  SAMPLE_BIOMATERIAL_PROVIDER_CH
  SAMPLE_TREATMENT_PROTOCOL_CH
  SAMPLE_GROWTH_PROTOCOL_CH
  SAMPLE_MOLECULE_CH
  SAMPLE_EXTRACT_PROTOCOL_CH
  SAMPLE_LABEL_CH
  SAMPLE_LABEL_PROTOCOL_CH
  SAMPLE_HYB_PROTOCOL
  SAMPLE_SCAN_PROTOCOL
  SAMPLE_DATA_PROCESSING
  SAMPLE_DESCRIPTION
  SAMPLE_PLATFORM_ID
  SAMPLE_GEO_ACCESSION
  SAMPLE_ANCHOR
  SAMPLE_TYPE
  SAMPLE_TAG_COUNT
  SAMPLE_TAG_LENGTH
  SERIES_TOKEN
  SERIES_TITLE
  SERIES_SUMMARY
  SERIES_OVERALL_DESIGN
  SERIES_PUBMED_ID
  SERIES_WEB_LINK
  SERIES_CONTRIBUTOR
  SERIES_VARIABLE
  SERIES_VARIABLE_DESCRIPTION
  SERIES_VARIABLE_SAMPLE_LIST
  SERIES_REPEATS
  SERIES_REPEATS_SAMPLE_LIST
  SERIES_SAMPLE_ID
  SERIES_GEO_ACCESSION
  
  PLATFORM_WEB_LINK
  SERIES_WEB_LINK
  
  SAMPLE_SUPPLEMENTARY_FILE
  SAMPLE_TABLE
  
  PLATFORM_TOKEN
  SAMPLE_TOKEN
  SERIES_TOKEN

TABLE:
  PLATFORM_TABLE_BEGIN
  PLATFORM_TABLE_END
  SAMPLE_TABLE_BEGIN
  SAMPLE_TABLE_END
  

  NUMBERED_KEY IS VALUE
  TABLE_START TABLE_DATA TABLE_END