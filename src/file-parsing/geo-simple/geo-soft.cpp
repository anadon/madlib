#include "geo-soft.hpp"
#include <string.h>

using std::string;
using std::vector;
using std::unordered_map;

//Due to the size of this function, it is being split into a section to create
//the intermediate representation, and then the parsing of that intermediate
//representation in another file, geo-soft.cpp.
int loadGeoSoftFile(const char *fp, struct GeoSoft **contents){
	int status;

	unordered_map<string, vector<vector<string> > > intermediate;
  intermediate = loadGeoSoftFileIR(fp);

  if(0 == intermediate.size()){
    #ifdef DEBUG
    fprintf(stderr, "ERROR: failed to get intermediate representation from "
                    "file\n");
    #endif
    //TODO: set errno as appropriate

    return 1;
  }

	if(NULL == *contents){
		*contents = new GeoSoft();
	}



	if(!intermediate.count(CSTRING_PLATFORM)){ //std::string
		(*contents)->platform = intermediate[string(CSTRING_PLATFORM)][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n", CSTRING_PLATFORM);
		#endif
		status++;
	}


	if(!intermediate.count(CSTRING_Platform_title)){ //std::string
		(*contents)->platform_title = intermediate[STRING_Platform_title][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																												CSTRING_Platform_title);
		#endif
		status++;
	}

	//enum platform_distribution_enum
	if(!intermediate.count(CSTRING_Platform_distribution)){
		(*contents)->platform_distribution = platform_distribution_unset;
		if(!strcmp(intermediate[STRING_Platform_distribution][0][0].c_str(),
																																"commercial")){
			(*contents)->platform_distribution = commercial;
		}else if(!strcmp(intermediate[STRING_Platform_distribution][0][0].c_str(),
																														"non-commercial")){
			(*contents)->platform_distribution = non_commercial;
		}else if(!strcmp(intermediate[STRING_Platform_distribution][0][0].c_str(),
																													"custom-commercial")){
			(*contents)->platform_distribution = custom_commercial;
		}else if(!strcmp(intermediate[STRING_Platform_distribution][0][0].c_str(),
																																		"virtual")){
			(*contents)->platform_distribution = _virtual;
		}else{
			#ifdef DEBUG
			fprintf(stderr, "ERROR: Value for key \"%s\" is invalid\n",
																								CSTRING_Platform_distribution);
			#endif
			status++;
		}

	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																								CSTRING_Platform_distribution);
		#endif
		status++;
	}


	//enum platform_technology_enum
	if(!intermediate.count(CSTRING_Platform_technology)){
		(*contents)->platform_technology = platform_technology_unset;
		if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																														"spotted DNA/cDNA")){
			(*contents)->platform_technology = spotted_DNA_or_cDNA;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																										"spotted oligonucleotide")){
			(*contents)->platform_technology = spotted_oligonucleotide;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																										"in situ oligonucleotide")){
			(*contents)->platform_technology = in_situ_oligonucleotide;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																																		"antibody")){
			(*contents)->platform_technology = antibody;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																																			"tissue")){
			(*contents)->platform_technology = tissue;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																																			"SARST")){
			(*contents)->platform_technology = SARST;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																																			"RT-PCR")){
			(*contents)->platform_technology = RT_PCR;
		}else if(!strcmp(intermediate[STRING_Platform_technology][0][0].c_str(),
																																				"MPSS")){
			(*contents)->platform_technology = MPSS;
		}else{
			#ifdef DEBUG
			fprintf(stderr, "ERROR: Value for key \"%s\" is invalid\n",
																								CSTRING_Platform_distribution);
			#endif
			status++;
		}
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																									CSTRING_Platform_technology);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Platform_organism)){
		(*contents)->platform_organism = intermediate[STRING_Platform_organism][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																										CSTRING_Platform_organism);
		#endif
		status++;
	}


	if(!intermediate.count(CSTRING_Platform_manufacturer)){  //std::string
		(*contents)->platform_manufacturer =
															intermediate[STRING_Platform_manufacturer][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																								CSTRING_Platform_manufacturer);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Platform_manufacture_protocol)){
		(*contents)->platform_manufacture_protocol =
												intermediate[STRING_Platform_manufacture_protocol][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																				CSTRING_Platform_manufacture_protocol);
		#endif
		status++;
	}

	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Platform_catalog_number)){
		(*contents)->platform_catalog_number =
															intermediate[STRING_Platform_catalog_number][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Platform_web_link)){
		(*contents)->platform_web_link =
																intermediate[STRING_Platform_web_link][0];
	}


	//std::string
	if(!intermediate.count(CSTRING_Platform_support)){
		(*contents)->platform_support =
																	intermediate[STRING_Platform_support][0][0];
	}


	//std::string
	if(!intermediate.count(CSTRING_Platform_coating)){
		(*contents)->platform_coating =
																	intermediate[STRING_Platform_coating][0][0];
	}


	//std::string
	if(!intermediate.count(CSTRING_Platform_description)){
		(*contents)->platform_description =
															intermediate[STRING_Platform_description][0][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Platform_contributor)){
		(*contents)->platform_contributor =
																	intermediate[STRING_Platform_contributor][0];
	}


	//std::vector<int>
	if(!intermediate.count(CSTRING_Platform_pubmed_id)){
		for(size_t i = 0;
								i < intermediate[STRING_Platform_pubmed_id][0].size(); i++){
			(*contents)->platform_pubmed_id.push_back(
													atoi(intermediate[STRING_Platform_pubmed_id][0][i].c_str()));
		}
	}


	//std::string
	if(!intermediate.count(CSTRING_Platform_geo_accession)){
		(*contents)->platform_geo_accession =
														intermediate[STRING_Platform_geo_accession][0][0];
	}


	if(!intermediate.count(CSTRING_Platform_table_begin)){
		(*contents)->platform_table_column_titles = intermediate[CSTRING_Platform_table_begin][0];
		intermediate[CSTRING_Platform_table_begin].erase(intermediate[CSTRING_Platform_table_begin].begin());
		(*contents)->platform_table_matrix = intermediate[CSTRING_Platform_table_begin];

		size_t numRows = (*contents)->platform_table_matrix.size();
		size_t numCols = (*contents)->platform_table_column_titles.size();

		//First, establish that the table is rectangular.
		bool colsConsistent = true;
		for(size_t y = 0; y < numRows; y++){
			if((*contents)->platform_table_matrix[y].size() != numCols){
				#ifdef DEBUG
				fprintf(stderr, "ERROR: \"%s\" mismatch in number of columns in rows\n",
																						CSTRING_Platform_table_begin);
				#endif
				status++;
				colsConsistent = false;
			}
		}

		if(colsConsistent){

			//check for single occurrence of certain column headers
			bool colHeaderPresent;
			const char *headerNames[] = {"ID",
			 														 "SEQUENCE",
																	 "ORGANISM",
																	 NULL};
			for(size_t i = 0; headerNames[i]; i++){
				colHeaderPresent = false;
				for(size_t x = 0; x < numCols; x++){
					if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), headerNames[i])){
						if(colHeaderPresent){
							#ifdef DEBUG
							fprintf(stderr, "ERROR: \"%s\" multiple occurance of column header \"%s\"\n",
																								CSTRING_Platform_table_begin, headerNames[i]);
							#endif
							status++;
						}else{
							colHeaderPresent = true;
						}
					}
				}
				if(!colHeaderPresent){

					#ifdef DEBUG
					fprintf(stderr, "ERROR: \"%s\" column header \"%s\" is missing\n",
																	CSTRING_Platform_table_begin, headerNames[i]);
					#endif
					status++;
				}
			}

			//TODO: this could be refined.
			//Per column validation
			for(size_t x = 0; x < numCols; x++){
				if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "ID")){
					//Datatype: std::string
					//Regex: *
					//Qualifications: unique in column, required, unique among column headers

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "SEQUENCE")){
					//Datatype: std::string
					//Regex: [ATCG]+
					//Qualifications: unique among column headers

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GB_ACC")){
					//Datatype: std::string
					//Regex: .+\.[0-9]+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GB_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GB_RANGE")){
					//Datatype: std::string
					//Regex: .+\.[0-9]+\[[0-9]+\.\.[0-9]+\]
					//Qualifications: start < end

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "RANGE_GB")){
					//Datatype: std::string
					//Regex: .+\.[0-9]+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "RANGE_START")){
					//Datatype: long int
					//Regex: [0-9]+
					//Qualifications: Implicitly requires RANGE_END, less than RANGE_END

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "RANGE_END")){
					//Datatype: long int
					//Regex: [0-9]+
					//Qualifications: Implicitly requires RANGE_START, greater than RANGE_START

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "RANGE_STRAND")){
					//Datatype: std:string
					//Regex: {\+}|{\-}|{}|{empty}

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GI")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GI_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GI_RANGE")){
					//Datatype: std::string
					//Regex: GI\[[0-9]+\.\.[0-9]+\]
					//Qualifications: First number is less than second

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "CLONE_ID")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "CLONE_ID_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "ORF")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "ORF_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "GENOME_ACC")){
					//Datatype: std::string
					//Regex: .+\.[0-9]+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "SNP_ID")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "SNP_ID_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "miRNA_ID")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "miRNA_ID_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "SPOT_ID")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "ORGANISM")){
					//Datatype: std::string
					//Regex: .+
					//Qualifications: unique among column headers

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "PT_ACC")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "PT_GI")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "PT_GI_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "SP_ACC")){
					//Datatype: std::string
					//Regex: .+

				}else if(0 == strcmp((*contents)->platform_table_column_titles[x].c_str(), "SP_LIST")){
					//Datatype: std::string
					//Regex: .+

				}else{
					//Datatype: std::string
					//NOTE: No idea what it is, just preserve it as is.

				}
			}
		}
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: Platform table entries missing\n");
		#endif
		status++;
	}


	//should be handled by the CSTRING_Platform_table_begin clause
	//if(!intermediate.count(CSTRING_Platform_table_end)){
	//  (*contents)->PLATFORM_TABLE_END
	//}else{
	//}


	//std::string
	if(!intermediate.count(CSTRING_SAMPLE)){\
		(*contents)->sample = intermediate[STRING_SAMPLE][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n", CSTRING_SAMPLE);
		#endif
		status++;
	}


	//std::string
	if(!intermediate.count(CSTRING_Sample_title)){
		(*contents)->sample_title = intermediate[STRING_Sample_title][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n", CSTRING_Sample_title);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Sample_supplementary_file)){
		(*contents)->sample_supplementary_file =
														intermediate[STRING_Sample_supplementary_file][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																						CSTRING_Sample_supplementary_file);
		#endif
		status++;
	}


	//std::string
	if(!intermediate.count(CSTRING_Sample_table)){
		(*contents)->sample_table = intermediate[STRING_Sample_table][0][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Sample_hyb_protocol)){
		(*contents)->sample_hyb_protocol = intermediate[STRING_Sample_hyb_protocol][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																									CSTRING_Sample_hyb_protocol);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Sample_scan_protocol)){
		(*contents)->sample_scan_protocol =
																	intermediate[STRING_Sample_scan_protocol][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																									CSTRING_Sample_scan_protocol);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Sample_data_processing)){
		(*contents)->sample_data_processing =
																intermediate[STRING_Sample_data_processing][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																								CSTRING_Sample_data_processing);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Sample_description)){
		(*contents)->sample_description =
																		intermediate[STRING_Sample_description][0];
	}


	//std::string
	if(!intermediate.count(CSTRING_Sample_platform_id)){
		(*contents)->sample_platform_id =
																intermediate[STRING_Sample_platform_id][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																										CSTRING_Sample_platform_id);
		#endif
		status++;
	}


	//std::string
	if(!intermediate.count(CSTRING_Sample_geo_accession)){
		(*contents)->sample_geo_accession =
															intermediate[STRING_Sample_geo_accession][0][0];
	}


	//std::string
	if(!intermediate.count(CSTRING_Sample_anchor)){
		(*contents)->sample_anchor = intermediate[STRING_Sample_anchor][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n", CSTRING_Sample_anchor);
		#endif
		status++;
	}


	//NOTE: SAGE submission needs to be block checked.
	//std::string
	if(!intermediate.count(CSTRING_Sample_type)){
		(*contents)->sample_type = intermediate[STRING_Sample_type][0][0];
	}


	//NOTE: SAGE submission needs to be block checked
	//int
	if(!intermediate.count(CSTRING_Sample_tag_count)){
		(*contents)->sample_tag_count =
														atoi(intermediate[STRING_Sample_tag_count][0][0].c_str());
	}


	//NOTE: SAGE submission needs to be block checked.
	//int
	if(!intermediate.count(CSTRING_Sample_tag_length)){
		(*contents)->sample_tag_length =
														atoi(intermediate[STRING_Sample_tag_length][0][0].c_str());
	}


	if(!intermediate.count(CSTRING_Sample_table_begin)){
		(*contents)->sample_table_columns = intermediate[STRING_Sample_table_begin][0];
		intermediate[STRING_Sample_table_begin][0].erase(intermediate[STRING_Sample_table_begin][0].begin());
		(*contents)->sample_table_matrix = intermediate[STRING_Sample_table_begin];

		//TODO: verify
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																										CSTRING_Sample_table_begin);
		#endif
		status++;
	}


	//Handled by CSTRING_Sample_table_begin
	//if(!intermediate.count(CSTRING_Sample_table_end)){
	//  (*contents)->SAMPLE_TABLE_END
	//}else{
	//}


	//std::string
	if(!intermediate.count(CSTRING_SERIES)){
		(*contents)->series = intermediate[STRING_SERIES][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n", CSTRING_SERIES);
		#endif
		status++;
	}


	//std::string
	if(!intermediate.count(CSTRING_Series_title)){
		(*contents)->series_title = intermediate[STRING_Series_title][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n", CSTRING_Series_title);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_summary)){
		(*contents)->series_summary = intermediate[STRING_Series_summary][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																												CSTRING_Series_summary);
		#endif
		status++;
	}


	//std::string;
	if(!intermediate.count(CSTRING_Series_overall_design)){
		(*contents)->series_overall_design =
															intermediate[STRING_Series_overall_design][0][0];
	}else{
		#ifdef DEBUG
		fprintf(stderr, "ERROR: \"%s\" missing from file\n",
																												CSTRING_Series_summary);
		#endif
		status++;
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_pubmed_id)){
		(*contents)->series_pubmed_id = intermediate[STRING_Series_pubmed_id][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_web_link)){
		(*contents)->series_web_link = intermediate[STRING_Series_web_link][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_contributor)){
		(*contents)->series_contributor =
																		intermediate[STRING_Series_contributor][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_variable_)){
		(*contents)->series_variable = intermediate[STRING_Series_variable_][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_variable_description_)){
		(*contents)->series_variable_description =
													intermediate[STRING_Series_variable_description_][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_variable_sample_list_)){
		(*contents)->series_variable_sample_list =
													intermediate[STRING_Series_variable_sample_list_][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_repeats_)){
		(*contents)->series_repeats = intermediate[STRING_Series_repeats_][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_repeats_sample_list_)){
		(*contents)->series_repeats_sample_list =
													intermediate[STRING_Series_repeats_sample_list_][0];
	}


	//std::vector<std::string>
	if(!intermediate.count(CSTRING_Series_sample_id)){
		(*contents)->series_sample_id = intermediate[STRING_Series_sample_id][0];
	}


	//std::string
	if(!intermediate.count(CSTRING_Series_geo_accession)){
		(*contents)->series_geo_accession =
															intermediate[STRING_Series_geo_accession][0][0];
	}


	//NOTE: channel checking must be done as a group
	//Channel block
	size_t lastIndex;
	lastIndex = intermediate.count(CSTRING_Sample_source_name_ch);

	if(lastIndex != intermediate.count(CSTRING_Sample_organism_ch) ||
		lastIndex != intermediate.count(CSTRING_Sample_characteristics_ch) ||
		lastIndex != intermediate.count(CSTRING_Sample_biomaterial_provider_ch) ||
			lastIndex != intermediate.count(CSTRING_Sample_treatment_protocol_ch) ||
				lastIndex != intermediate.count(CSTRING_Sample_growth_protocol_ch) ||
				lastIndex != intermediate.count(CSTRING_Sample_molecule_ch) ||
				lastIndex != intermediate.count(CSTRING_Sample_extract_protocol_ch) ||
						lastIndex != intermediate.count(CSTRING_Sample_label_ch) ||
						lastIndex != intermediate.count(CSTRING_Sample_label_protocol_ch)){
		status++;

		//Find the max value, to try and recover as much information as possible
		//and allocate appropriately.
		size_t highCheck;
		highCheck = intermediate.count(CSTRING_Sample_organism_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_characteristics_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_biomaterial_provider_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_treatment_protocol_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_growth_protocol_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_molecule_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_extract_protocol_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_label_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
		highCheck = intermediate.count(CSTRING_Sample_label_protocol_ch);
		if(lastIndex < highCheck) lastIndex = highCheck;
	}

	//std::vector<std::string>
	for(size_t i = 0; i < lastIndex; i++){
		struct GeoSoftChannel tmp;
		(*contents)->channel.push_back(tmp);
	}

	for(size_t i = 0; i < intermediate[STRING_Sample_source_name_ch][0].size(); i++)
		(*contents)->channel[i].sample_source_name =
															intermediate[STRING_Sample_source_name_ch][0][i];

	for(size_t i = 0; i < intermediate[STRING_Sample_organism_ch].size(); i++)
		(*contents)->channel[i].sample_organism =
																		intermediate[STRING_Sample_organism_ch][i];

	//NOTE: channel checking must be done as a group
	for(size_t i = 0; i < intermediate[STRING_Sample_characteristics_ch].size(); i++){
		for(size_t j = 0; j < intermediate[STRING_Sample_characteristics_ch][i].size(); j++){
			std::string tag, value, both;
			both = intermediate[STRING_Sample_characteristics_ch][i][j];
			tag = both.substr(0, both.find(":"));
			value = both.substr(both.find(":") + 1);
			std::pair<std::string, std::string> tagVal = {tag, value};
			(*contents)->channel[i].sample_characteristics.push_back(tagVal);
		}
	}


	//std::vector<std::vector<std::string> >
	for(int i = 0; intermediate.count(CSTRING_Sample_biomaterial_provider_ch); i++){
		(*contents)->channel[i].sample_biomaterial_provider =
											intermediate[STRING_Sample_biomaterial_provider_ch][i];
	}


	//std::vector<std::vector<std::string> >
	for(int i = 0; i < intermediate.count(CSTRING_Sample_treatment_protocol_ch); i++){
		(*contents)->channel[i].sample_treatment_protocol =
														intermediate[STRING_Sample_treatment_protocol_ch][i];
	}


	//std::vector<std::vector<std::string> >
	for(int i = 0; i < intermediate.count(CSTRING_Sample_growth_protocol_ch); i++){
		(*contents)->channel[i].sample_growth_protocol =
																intermediate[STRING_Sample_growth_protocol_ch][i];
	}

	//std::vector<std::vector<std::string> >
	for(int i = 0; i < intermediate.count(CSTRING_Sample_extract_protocol_ch); i++){
		(*contents)->channel[i].sample_extract_protocol =
																intermediate[STRING_Sample_growth_protocol_ch][i];
	}

	//std::vector<std::string>
	for(int i = 0; i < intermediate.count(CSTRING_Sample_label_ch); i++){
		(*contents)->channel[i].sample_label =
																intermediate[STRING_Sample_label_ch][i][0];
	}


	//std::vector<std::vector<std::string> >
	for(int i = 0; i < intermediate.count(CSTRING_Sample_label_protocol_ch); i++){
		(*contents)->channel[i].sample_label_protocol =
																intermediate[STRING_Sample_label_protocol_ch][i];
	}


	//SAGE block check here
	if((intermediate.count(CSTRING_Sample_anchor)     == 1 &&
			intermediate.count(CSTRING_Sample_type)       == 1 &&
			intermediate.count(CSTRING_Sample_tag_count)  == 1 &&
			intermediate.count(CSTRING_Sample_tag_length) == 1  ) ||
		 (intermediate.count(CSTRING_Sample_anchor)     == 0 &&
			intermediate.count(CSTRING_Sample_type)       == 0 &&
			intermediate.count(CSTRING_Sample_tag_count)  == 0 &&
			intermediate.count(CSTRING_Sample_tag_length) == 0)){
		status++;
	}

	//std::string
	if(!intermediate.count(CSTRING_Sample_anchor)){
		(*contents)->sample_anchor = intermediate[STRING_Sample_anchor][0][0];
	}

	//std::string
	if(!intermediate.count(CSTRING_Sample_type)){
		(*contents)->sample_type = intermediate[STRING_Sample_type][0][0];
	}

	//std::string
	if(!intermediate.count(CSTRING_Sample_tag_count)){
		(*contents)->sample_tag_count = atoi(intermediate[STRING_Sample_tag_count][0][0].c_str());
	}

	//std::string
	if(!intermediate.count(CSTRING_Sample_tag_length)){
		(*contents)->sample_tag_length = atoi(intermediate[STRING_Sample_tag_length][0][0].c_str());
	}

	return status;
}
