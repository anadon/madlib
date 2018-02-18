%option noyywrap
%option warn
%option yylineno
%option header-file="lex.yy.h"
%option bison-bridge reentrant bison-locations

%{
  #include <string.h>
  #include "geo-soft.tab.hh"
  //#include "lex.yy.h"
%}

/*URL specification at https://www.ietf.org/rfc/rfc1738.txt
Regex courtesy of @diegoperini via https://mathiasbynens.be/demo/url-regex */
/*small block courtesy*/
/*https://stackoverflow.com/questions/14462808/flex-bison-interpreting-numbers-as-floats*/
DIGIT [0-9]
INTEGER {DIGIT}+
REAL ({DIGIT}+[.]{DIGIT}*)|({DIGIT}*[.]{DIGIT}+)


IS[ \t]*=[ \t]*

PLATFORM_TOKEN                  "\[^]PLATFORM"
PLATFORM_TITLE                  "!Platform_title"
PLATFORM_DISTRIBUTION           "!Platform_distribution"
PLATFORM_TECHNOLOGY             "!Platform_technology"
PLATFORM_ORGANISM               "!Platform_organism"
PLATFORM_MANUFACTURER           "!Platform_manufacturer"
PLATFORM_MANUFACTURE_PROTOCOL   "!Platform_manufacture_protocol"
PLATFORM_CATALOG_NUMBER         "!Platform_catalog_number"
PLATFORM_WEB_LINK               "!Platform_web_link"
PLATFORM_SUPPORT                "!Platform_support"
PLATFORM_COATING                "!Platform_coating"
PLATFORM_DESCRIPTION            "!Platform_description"
PLATFORM_CONTRIBUTOR            "!Platform_contributor"
PLATFORM_PUBMED_ID              "!Platform_pubmed_id"
PLATFORM_GEO_ACCESSION          "!Platform_geo_accession"
PLATFORM_TABLE_BEGIN            "!Platform_table_begin"
PLATFORM_TABLE_END              "!Platform_table_end"
SAMPLE_TOKEN                    "\[^]SAMPLE"
SAMPLE_TITLE                    "!Sample_title"
SAMPLE_SUPPLEMENTARY_FILE       "!Sample_supplementary_file"
SAMPLE_TABLE                    "!Sample_table"
SAMPLE_SOURCE_NAME_CH           "!Sample_source_name_ch"
SAMPLE_ORGANISM_CH              "!Sample_organism_ch"
SAMPLE_CHARACTERISTICS_CH       "!Sample_characteristics_ch"
SAMPLE_BIOMATERIAL_PROVIDER_CH  "!Sample_biomaterial_provider_ch"
SAMPLE_TREATMENT_PROTOCOL_CH    "!Sample_treatment_protocol_ch"
SAMPLE_GROWTH_PROTOCOL_CH       "!Sample_growth_protocol_ch"
SAMPLE_MOLECULE_CH              "!Sample_molecule_ch"
SAMPLE_EXTRACT_PROTOCOL_CH      "!Sample_extract_protocol_ch"
SAMPLE_LABEL_CH                 "!Sample_label_ch"
SAMPLE_LABEL_PROTOCOL_CH        "!Sample_label_protocol_ch"
SAMPLE_HYB_PROTOCOL             "!Sample_hyb_protocol"
SAMPLE_SCAN_PROTOCOL            "!Sample_scan_protocol"
SAMPLE_DATA_PROCESSING          "!Sample_data_processing"
SAMPLE_DESCRIPTION              "!Sample_description"
SAMPLE_PLATFORM_ID              "!Sample_platform_id"
SAMPLE_GEO_ACCESSION            "!Sample_geo_accession"
SAMPLE_ANCHOR                   "!Sample_anchor"
SAMPLE_TYPE                     "!Sample_type"
SAMPLE_TAG_COUNT                "!Sample_tag_count"
SAMPLE_TAG_LENGTH               "!Sample_tag_length"
SAMPLE_TABLE_BEGIN              "!Sample_table_begin"
SAMPLE_TABLE_END                "!Sample_table_end"
SERIES_TOKEN                    "\[^]SERIES"
SERIES_TITLE                    "!Series_title"
SERIES_SUMMARY                  "!Series_summary"
SERIES_OVERALL_DESIGN           "!Series_overall_design"
SERIES_PUBMED_ID                "!Series_pubmed_id"
SERIES_WEB_LINK                 "!Series_web_link"
SERIES_CONTRIBUTOR              "!Series_contributor"
SERIES_VARIABLE                 "!Series_variable_"
SERIES_VARIABLE_DESCRIPTION     "!Series_variable_description_"
SERIES_VARIABLE_SAMPLE_LIST     "!Series_variable_sample_list_"
SERIES_REPEATS                  "!Series_repeats_"
SERIES_REPEATS_SAMPLE_LIST      "!Series_repeats_sample_list_"
SERIES_SAMPLE_ID                "!Series_sample_id"
SERIES_GEO_ACCESSION            "!Series_geo_accession"

COMMENT "#[ -!]*"
WORD  "[!-~]+"
VALUE "([!-~][ -!]*[!-~] | [!-~])"
WHITE_SPACE "[ \\t]*"
EOF "<<EOF>>"
LINE_END "WHITE_SPACE*$"

RESEARCHER_NAME                 "[A-Z][a-z]*,([A-Z],)?[A-Z][a-z]*"
GSE_NUMBER "GSE[0-9]+"
GPL_NUMBER "GPL[0-9]+"
GSM_NUMBER "GSM[0-9]+"

URL " _^(?:(?:https?|ftp)://)(?:\S+(?::\S*)?@)?(?:(?!10(?:\.\d{1,3}){3})(?!127(?:\.\d{1,3}){3})(?!169\.254(?:\.\d{1,3}){2})(?!192\.168(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:(?:[a-z\x{00a1}-\x{ffff}0-9]+-?)*[a-z\x{00a1}-\x{ffff}0-9]+)(?:\.(?:[a-z\x{00a1}-\x{ffff}0-9]+-?)*[a-z\x{00a1}-\x{ffff}0-9]+)*(?:\.(?:[a-z\x{00a1}-\x{ffff}]{2,})))(?::\d{2,5})?(?:/[^\s]*)?$_iuS"

TAG_VALUE "[!-9,;-~]+:[!-9,;-~]+"


%%

{PLATFORM_TOKEN} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_TOKEN;}
{PLATFORM_TITLE} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_TITLE;}
{PLATFORM_DISTRIBUTION} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_DISTRIBUTION;}
{PLATFORM_TECHNOLOGY} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_TECHNOLOGY;}
{PLATFORM_ORGANISM} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_ORGANISM;}
{PLATFORM_MANUFACTURER} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_MANUFACTURER;}
{PLATFORM_MANUFACTURE_PROTOCOL} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_MANUFACTURE_PROTOCOL;}
{PLATFORM_CATALOG_NUMBER} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_TOKEN;}
{PLATFORM_WEB_LINK} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_WEB_LINK;}
{PLATFORM_SUPPORT} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_SUPPORT;}
{PLATFORM_COATING} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_COATING;}
{PLATFORM_DESCRIPTION} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_DESCRIPTION;}
{PLATFORM_CONTRIBUTOR} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_CONTRIBUTOR;}
{PLATFORM_PUBMED_ID} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_PUBMED_ID;}
{PLATFORM_GEO_ACCESSION} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_GEO_ACCESSION;}
{PLATFORM_TABLE_BEGIN} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_TABLE_BEGIN;}
{PLATFORM_TABLE_END} {yylval->s = new std::string(yyg->yytext_r); return PLATFORM_TABLE_END;}
{SAMPLE_TOKEN} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TOKEN;}
{SAMPLE_TITLE} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TITLE;}
{SAMPLE_SUPPLEMENTARY_FILE} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_SUPPLEMENTARY_FILE;}
{SAMPLE_TABLE} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TABLE;}
{SAMPLE_SOURCE_NAME_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_SOURCE_NAME_CH;}
{SAMPLE_ORGANISM_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_ORGANISM_CH;}
{SAMPLE_CHARACTERISTICS_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_CHARACTERISTICS_CH;}
{SAMPLE_BIOMATERIAL_PROVIDER_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_BIOMATERIAL_PROVIDER_CH;}
{SAMPLE_TREATMENT_PROTOCOL_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TREATMENT_PROTOCOL_CH;}
{SAMPLE_GROWTH_PROTOCOL_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_GROWTH_PROTOCOL_CH;}
{SAMPLE_MOLECULE_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_MOLECULE_CH;}
{SAMPLE_EXTRACT_PROTOCOL_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_EXTRACT_PROTOCOL_CH;}
{SAMPLE_LABEL_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_LABEL_CH;}
{SAMPLE_LABEL_PROTOCOL_CH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_LABEL_PROTOCOL_CH;}
{SAMPLE_HYB_PROTOCOL} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_HYB_PROTOCOL;}
{SAMPLE_SCAN_PROTOCOL} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_SCAN_PROTOCOL;}
{SAMPLE_DATA_PROCESSING} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_DATA_PROCESSING;}
{SAMPLE_DESCRIPTION} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_DESCRIPTION;}
{SAMPLE_PLATFORM_ID} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_PLATFORM_ID;}
{SAMPLE_GEO_ACCESSION} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_GEO_ACCESSION;}
{SAMPLE_ANCHOR} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_ANCHOR;}
{SAMPLE_TYPE} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TYPE;}
{SAMPLE_TAG_COUNT} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TAG_COUNT;}
{SAMPLE_TAG_LENGTH} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TAG_LENGTH;}
{SAMPLE_TABLE_BEGIN} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TABLE_BEGIN;}
{SAMPLE_TABLE_END} {yylval->s = new std::string(yyg->yytext_r); return SAMPLE_TABLE_END;}
{SERIES_TOKEN} {yylval->s = new std::string(yyg->yytext_r); return SERIES_TOKEN;}
{SERIES_TITLE} {yylval->s = new std::string(yyg->yytext_r); return SERIES_TITLE;}
{SERIES_SUMMARY} {yylval->s = new std::string(yyg->yytext_r); return SERIES_SUMMARY;}
{SERIES_OVERALL_DESIGN} {yylval->s = new std::string(yyg->yytext_r); return SERIES_OVERALL_DESIGN;}
{SERIES_PUBMED_ID} {yylval->s = new std::string(yyg->yytext_r); return SERIES_PUBMED_ID;}
{SERIES_WEB_LINK} {yylval->s = new std::string(yyg->yytext_r); return SERIES_WEB_LINK;}
{SERIES_CONTRIBUTOR} {yylval->s = new std::string(yyg->yytext_r); return SERIES_CONTRIBUTOR;}
{SERIES_VARIABLE} {yylval->s = new std::string(yyg->yytext_r); return SERIES_VARIABLE;}
{SERIES_VARIABLE_DESCRIPTION} {yylval->s = new std::string(yyg->yytext_r); return SERIES_VARIABLE_DESCRIPTION;}
{SERIES_VARIABLE_SAMPLE_LIST} {yylval->s = new std::string(yyg->yytext_r); return SERIES_VARIABLE_SAMPLE_LIST;}
{SERIES_REPEATS} {yylval->s = new std::string(yyg->yytext_r); return SERIES_REPEATS;}
{SERIES_REPEATS_SAMPLE_LIST} {yylval->s = new std::string(yyg->yytext_r); return SERIES_REPEATS_SAMPLE_LIST;}
{SERIES_SAMPLE_ID} {yylval->s = new std::string(yyg->yytext_r); return SERIES_SAMPLE_ID;}
{SERIES_GEO_ACCESSION} {yylval->s = new std::string(yyg->yytext_r); return SERIES_GEO_ACCESSION;}

{COMMENT}
{VALUE} {yylval->s = new std::string(yyg->yytext_r); return VALUE;}
{WHITE_SPACE}
{EOF}
{LINE_END}


{RESEARCHER_NAME} {yylval->s = new std::string(yyg->yytext_r); return RESEARCHER_NAME;}
{GSE_NUMBER} {yylval->s = new std::string(yyg->yytext_r); return GSE_NUMBER;}
{GPL_NUMBER} {yylval->s = new std::string(yyg->yytext_r); return GPL_NUMBER;}
{GSM_NUMBER} {yylval->s = new std::string(yyg->yytext_r); return GSM_NUMBER;}


{URL} {yylval->s = new std::string(yyg->yytext_r); return URL;}

{TAG_VALUE} {yylval->s = new std::string(yyg->yytext_r); return TAG_VALUE;}

%%
