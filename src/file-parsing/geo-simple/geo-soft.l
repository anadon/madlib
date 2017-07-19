
%option noyywrap
%option warn
%option yylineno
%option header-file="lex.yy.h"
%option bison-bridge reentrant bison-locations

%{
  #include <string.h>
  #include "geo-soft.tab.hh"
%}

/*URL specification at https://www.ietf.org/rfc/rfc1738.txt
Regex courtesy of @diegoperini via https://mathiasbynens.be/demo/url-regex */
/*small block courtesy*/
/*https://stackoverflow.com/questions/14462808/flex-bison-interpreting-numbers-as-floats*/
DIGIT [0-9]
INTEGER {DIGIT}+
REAL ({DIGIT}+[.]{DIGIT}*)|({DIGIT}*[.]{DIGIT}+)

%%

IS[ \t]*=[ \t]*

PLATFORM_TOKEN                  [^]PLATFORM
PLATFORM_TITLE                  !Platform_title
PLATFORM_DISTRIBUTION           !Platform_distribution
PLATFORM_TECHNOLOGY             !Platform_technology
PLATFORM_ORGANISM               !Platform_organism
PLATFORM_MANUFACTURER           !Platform_manufacturer
PLATFORM_MANUFACTURE_PROTOCOL   !Platform_manufacture_protocol
PLATFORM_CATALOG_NUMBER         !Platform_catalog_number
PLATFORM_WEB_LINK               !Platform_web_link
PLATFORM_SUPPORT                !Platform_support
PLATFORM_COATING                !Platform_coating
PLATFORM_DESCRIPTION            !Platform_description
PLATFORM_CONTRIBUTOR            !Platform_contributor
PLATFORM_PUBMED_ID              !Platform_pubmed_id
PLATFORM_GEO_ACCESSION          !Platform_geo_accession
PLATFORM_TABLE_BEGIN            !Platform_table_begin
PLATFORM_TABLE_END              !Platform_table_end
SAMPLE_TOKEN                    [^]SAMPLE
SAMPLE_TITLE                    !Sample_title
SAMPLE_SUPPLEMENTARY_FILE       !Sample_supplementary_file
SAMPLE_TABLE                    !Sample_table
SAMPLE_SOURCE_NAME_CH           !Sample_source_name_ch
SAMPLE_ORGANISM_CH              !Sample_organism_ch
SAMPLE_CHARACTERISTICS_CH       !Sample_characteristics_ch
SAMPLE_BIOMATERIAL_PROVIDER_CH  !Sample_biomaterial_provider_ch
SAMPLE_TREATMENT_PROTOCOL_CH    !Sample_treatment_protocol_ch
SAMPLE_GROWTH_PROTOCOL_CH       !Sample_growth_protocol_ch
SAMPLE_MOLECULE_CH              !Sample_molecule_ch
SAMPLE_EXTRACT_PROTOCOL_CH      !Sample_extract_protocol_ch
SAMPLE_LABEL_CH                 !Sample_label_ch
SAMPLE_LABEL_PROTOCOL_CH        !Sample_label_protocol_ch
SAMPLE_HYB_PROTOCOL             !Sample_hyb_protocol
SAMPLE_SCAN_PROTOCOL            !Sample_scan_protocol
SAMPLE_DATA_PROCESSING          !Sample_data_processing
SAMPLE_DESCRIPTION              !Sample_description
SAMPLE_PLATFORM_ID              !Sample_platform_id
SAMPLE_GEO_ACCESSION            !Sample_geo_accession
SAMPLE_ANCHOR                   !Sample_anchor
SAMPLE_TYPE                     !Sample_type
SAMPLE_TAG_COUNT                !Sample_tag_count
SAMPLE_TAG_LENGTH               !Sample_tag_length
SAMPLE_TABLE_BEGIN              !Sample_table_begin
SAMPLE_TABLE_END                !Sample_table_end
SERIES_TOKEN                    [^]SERIES
SERIES_TITLE                    !Series_title
SERIES_SUMMARY                  !Series_summary
SERIES_OVERALL_DESIGN           !Series_overall_design
SERIES_PUBMED_ID                !Series_pubmed_id
SERIES_WEB_LINK                 !Series_web_link
SERIES_CONTRIBUTOR              !Series_contributor
SERIES_VARIABLE                 !Series_variable_
SERIES_VARIABLE_DESCRIPTION     !Series_variable_description_
SERIES_VARIABLE_SAMPLE_LIST     !Series_variable_sample_list_
SERIES_REPEATS                  !Series_repeats_
SERIES_REPEATS_SAMPLE_LIST      !Series_repeats_sample_list_
SERIES_SAMPLE_ID                !Series_sample_id
SERIES_GEO_ACCESSION            !Series_geo_accession

COMMENT #[ -!]*
WORD  [!-~]+
VALUE ([!-~][ -!]*[!-~] | [!-~])
WHITE_SPACE [ \\t]*
EOF <<EOF>>
LINE_END WHITE_SPACE*$


RESEARCHER_NAME                 [A-Z][a-z]*,([A-Z],)?[A-Z][a-z]*
GSE_NUMBER GSE[0-9]+
GPL_NUMBER GPL[0-9]+
GSM_NUMBER GSM[0-9]+


URL _^(?:(?:https?|ftp)://)(?:\S+(?::\S*)?@)?(?:(?!10(?:\.\d{1,3}){3})(?!127(?:\.\d{1,3}){3})(?!169\.254(?:\.\d{1,3}){2})(?!192\.168(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:(?:[a-z\x{00a1}-\x{ffff}0-9]+-?)*[a-z\x{00a1}-\x{ffff}0-9]+)(?:\.(?:[a-z\x{00a1}-\x{ffff}0-9]+-?)*[a-z\x{00a1}-\x{ffff}0-9]+)*(?:\.(?:[a-z\x{00a1}-\x{ffff}]{2,})))(?::\d{2,5})?(?:/[^\s]*)?$_iuS

TAG_VALUE [!-9,;-~]+:[!-9,;-~]+

%%
