%defines
%locations
%start topLevelParseRule
%require "3.0.4"
%define api.pure full
%token-table
%param   {void *yyscanner}


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

	#ifndef YY_TYPEDEF_YY_SCANNER_T
	#define YY_TYPEDEF_YY_SCANNER_T
	typedef void* yyscan_t;
	#endif

	extern int yylex \
							 (YYSTYPE * yylval_param, YYLTYPE * yylloc_param , yyscan_t yyscanner);
	#define YY_DECL int yylex \
							 (YYSTYPE * yylval_param, YYLTYPE * yylloc_param , yyscan_t yyscanner)
	//#include "lex.yy.h"

	//These are gross hacks.
	static bool validFile = true;
	static std::unordered_map<std::string, std::vector<std::vector<std::string> > > *intermediate;

	void ensureSpaceForKey(
									unordered_map<string, vector<vector<string> > > &intermediate,
																																		string key);

	bool isUniqueInsert(
									unordered_map<string, vector<vector<string> > > &intermediate,
										string key, string value, int first_line, int first_column,
																															int last_column);

	bool isUniqueInsertForChannel(
									unordered_map<string, vector<vector<string> > > &intermediate,
													string key, string value, int channel, int first_line,
																						 int first_column, int last_column);

	void yyerror(YYLTYPE *yylloc, yyscan_t yyscanner, const char *msg);

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
		int index = atoi($2->c_str());
		string value = *$4;

			ensureSpaceForKey(*intermediate, key);

			while(intermediate->at(key).size() <= index)
				intermediate->at(key).push_back(std::vector<string>());

			intermediate->at(key).at(index).push_back(*$4);
		}

	| key_indexed_1 INTEGER IS VALUE {
		string key = *$1;
		int index = atoi($2->c_str());
		string value = *$4;

			ensureSpaceForKey(*intermediate, key);

			while(intermediate->at(key).size() <= index)
				intermediate->at(key).push_back(std::vector<string>());

			if(!isUniqueInsertForChannel(*intermediate, key, *$4, index, @4.first_line,
															@4.first_column, @4.last_column)) return 1;

			intermediate->at(key).at(index).push_back(*$4);
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
		string value = *$3;

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
		string value = *$3;

			ensureSpaceForKey(*intermediate, key);

			if(!isUniqueInsert(*intermediate, key, value, @3.first_line, @3.first_column,
																								@3.last_column)) return 1;

			intermediate->at(key).at(0).push_back(value);

		}

	| key_indexed_inf_tag_val INTEGER IS tag_value_list {
		string key = *$1;
		int index = atoi($2->c_str());
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

void yyerror(YYLTYPE *yylloc, yyscan_t yyscanner, const char *msg){

}


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
		fprintf(stderr, "Error: repeat insertion of key \"%s\" "
										"on line %d from columns %d-%d\n", key.c_str(), first_line,
																										first_column, last_column);
		return false;
	}

	bool isUniqueInsertForChannel(
									unordered_map<string, vector<vector<string> > > &intermediate,
													string key, string value, int channel, int first_line,
																						 int first_column, int last_column){
		if(0 == intermediate.count(key)) return true;
		if(0 == intermediate.at(key).at(channel).size()) return true;
		fprintf(stderr, "Error: repeat insertion of key \"%s\" with value "
										"on line %d from columns %d-%d\n", key.c_str(), first_line,
																										first_column, last_column);
		return false;
	}



#include "geo-soft.hpp"
#include "lex.yy.h"

//Load GEO's Soft file format intermediate representation
unordered_map<string, vector<vector<string> > > loadGeoSoftFileIR(
																																const char *fp){
	int status;
	FILE *FPCheck;
	unordered_map<string, vector<vector<string> > > tr;

	FPCheck = freopen(fp, "r", stdin);
	if(NULL == FPCheck)  return tr;
	stdin = FPCheck;

	intermediate = new unordered_map<string, vector<vector<string> > >();

	yyscan_t scanner;
	yylex_init(&scanner);
	yyparse(scanner);
	yylex_destroy(scanner);

	//return a copy, not the actual.
	tr = *intermediate;
	delete intermediate;
	return tr;
}
