#ifndef GEO_SIMPLE_H
#define GEO_SIMPLE_H

#include <string.h>
#include <vector>


enum platformHeaderTypes {ID, SEQUENCE, GB_ACC, GB_LIST, GB_RANGE, 
  RANGE_GB, RANGE_START, RANGE_END, RANGE_STRAND, GI, GI_LIST, GI_RANGE, 
  CLONE_ID, CLONE_ID_LIST, ORF, ORF_LIST, GENOME_ACC, SNP_ID, 
  SNP_ID_LIST, miRNA_ID, miRNA_ID_LIST, SPOT_ID, ORGANISM, PT_ACC, 
  PT_LIST, PT_GI, PT_GI_LIST, SP_ACC, SP_LIST};
/*******************************************************************//**
 * Column Label: ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SEQUENCE
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GB_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GB_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GB_RANGE
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_GB
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_START
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_END
 * Data Type   : 
 * Description : 
 * 
 * Column Label: RANGE_STRAND
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GI
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GI_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GI_RANGE
 * Data Type   : 
 * Description : 
 * 
 * Column Label: CLONE_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: CLONE_ID_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: ORF
 * Data Type   : 
 * Description : 
 * 
 * Column Label: ORF_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: GENOME_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SNP_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SNP_ID_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: miRNA_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: miRNA_ID_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SPOT_ID
 * Data Type   : 
 * Description : 
 * 
 * Column Label: ORGANISM
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_GI
 * Data Type   : 
 * Description : 
 * 
 * Column Label: PT_GI_LIST
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SP_ACC
 * Data Type   : 
 * Description : 
 * 
 * Column Label: SP_LIST
 * Data Type   : 
 * Description : 
 * 
***********************************************************************/

struct platformTable{
  size_t numCols;
  enum platformHeaderTypes *headers;
  void **data;
};


enum sampleHeaderTypes{ID_REF, VALUE};
/*******************************************************************//**
 * Column Label: ID_REF
 * Data Type   : 
 * Description : 
 * 
 * Column Label: VALUE
 * Data Type   : 
 * Description : 
 * 
***********************************************************************/


struct sampleTable{
  size_t numCols;
  enum sampleHeaderTypes *headers;
  void **data
};


struct GEOSimpleFile{
  std::string PLATFORM;
  /* Label/Key         : ^PLATFORM
   * Number per file   : 1
   * Value constraints : Unique key, ASCII
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records. 
   ********************************************************************/
  
  std::string Platform_title;
  /* Label/Key         : !Platform_title
   * Number per file   : 1
   * Value constraints : string of length 1-120 characters, must be 
   * unique within local file and over all previously submitted 
   * Platforms for that submitter.
   * 
   * Notes: Provide a unique title that describes 
   * your Platform. We suggest that you use the system 
   * [institution/lab]-[species]-[number of features]-[version], e.g. 
   * "FHCRC Mouse 15K v1.0".
   ********************************************************************/
  
  std::string Platform_distribution;
  /* Label/Key         : !Platform_distribution
   * Number per file   : 1
   * Value constraints : in {commercial, non-commercial, 
   *                     custom-commercial}
   * 
   * Notes: Should be in accordance with how the array was manufactured. 
   * Use 'virtual' only if creating a virtual definition for MPSS, 
   * SARST, or RT-PCR data.
   ********************************************************************/

  
  std::string Platform_technology;
  /* Label/Key         : !Platform_technology
   * Number per file   : 1
   * Value constraints : in {spotted DNA/cDNA, spotted oligonucleotide, 
   *                     in situ oligonucleotide, antibody, tissue, 
   *                     SARST, RT-PCR, or MPSS}
   * 
   * Notes: Select the category that best describes the Platform 
   * technology.
   ********************************************************************/
  
  std::vector<std::string> Platform_organism;
  /* Label/Key         : !Platform_organism
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Identify the organism(s) from which the features on the 
   * Platform were designed or derived.
   ********************************************************************/
  
  std::string Platform_manufacturer;
  /* Label/Key         : !Platform_manufacturer
   * Number per file   : 1
   * Value constraints : ASCII
   * 
   * Notes: Provide the name of the company, facility or laboratory 
   * where the array was manufactured or produced.
   ********************************************************************/
  
  std::vector<std::string> Platform_manufacture_protocol;
  /* Label/Key         : !Platform_manufacture_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Describe the array manufacture protocol. Include as much 
   * detail as possible, e.g., clone/primer set identification and 
   * preparation, strandedness/length, arrayer hardware/software, 
   * spotting protocols. You can include as much text as you need to 
   * thoroughly describe the protocol; it is strongly recommended that 
   * complete protocol descriptions are provided within your submission.
   ********************************************************************/
  
  std::vector<std::string> Platform_catalog_number;
  /* Label/Key         : !Platform_catalog_number
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes:Provide the manufacturer catalog number for 
   * commercially-available arrays.
   ********************************************************************/
  
  std::vector<std::string> Platform_web_link;
  /* Label/Key         : !Platform_web_link
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, valid URL
   * 
   * Notes: Specify a Web link that directs users to supplementary 
   * information about the array. Please restrict to Web sites that you 
   * know are stable.
   ********************************************************************/
  
  std::string Platform_support;
  /* Label/Key         : !Platform_support
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   * 
   * Notes: Provide the surface type of the array, e.g., glass, 
   * nitrocellulose, nylon, silicon, unknown.
   ********************************************************************/
  
  std::string Platform_coating;
  /* Label/Key         : !Platform_coating
   * Number per file   : [0, 1]
   * Value constraints : ASCII
   * 
   * Notes: Provide the coating of the array, e.g., aminosilane, quartz, 
   * polysine, unknown.
   ********************************************************************/
  
  std::vector<std::string> Platform_description;
  /* Label/Key         : !Platform_description
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Provide any additional descriptive information not captured 
   * in another field, e.g., array and/or feature physical dimensions, 
   * element grid system.
   ********************************************************************/
  
  std::vector<std::string> Platform_contributor;
  /* Label/Key         : !Platform_contributor
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, List all people associated with this 
   *                     array design.
   * 
   * Notes: Each value in the form 'firstname,middleinitial,lastname' or 
   * 'firstname,lastname': firstname must be at least one character and 
   * cannot contain spaces; middleinitial, if present, is one character; 
   * lastname is at least two characters and can contain spaces. 
   ********************************************************************/
  
  std::vector<std::string> Platform_pubmed_id;
  /* Label/Key         : !Platform_pubmed_id
   * Number per file   : [0, Infinity)
   * Value constraints : Integer, see notes.
   * 
   * Notes: Specify a valid PubMed identifier (PMID) that references a 
   * published article that describes the array.
   ********************************************************************/
  
  std::string Platform_geo_accession;
  /* Label/Key         : !Platform_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Integer, see notes.
   * 
   * Notes: A valid Platform accession number (GPLxxx) Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
  
  struct Platform_table platformTable;
  /* Label/Key         : !Platform_table_begin
   * Number per file   : 1
   * Value constraints : N/A, See notes.
   * 
   * Notes: No content required, indicates the start of the data table.
   * Until !Platform_table_end is reached, a tab seperated va;ue table
   * is described.
   ********************************************************************/
  /* Label/Key         : !Platform_table_end
   * Number per file   : 1
   * Value constraints : N/A, See notes.
   * 
   * Notes: Ends table section opened by !Platform_table_begin.
   ********************************************************************/
  
  //////////////////////////////////////////////////////////////////////
  
  std::string SAMPLE;
  /* Label/Key         : ^SAMPLE
   * Number per file   : 1
   * Value constraints : ASCII, Unique within file
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records.
   ********************************************************************/
   
  std::string Sample_title;
  /* Label/Key         : !Sample_title
   * Number per file   : 1
   * Value constraints : ASCII, [1-120] characters, unqiue in file and
   *                     submitter.
   * 
   * Notes: Provide a unique title that describes this Sample. We 
   * suggest that you use the system 
   * [biomaterial]-[condition(s)]-[replicate number], e.g., 
   * Muscle_exercised_60min_rep2.
   ********************************************************************/
   
  std::vector<std::string> Sample_supplementary_file;
  /* Label/Key         : !Sample_supplementary_file
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Name of supplementary file, or 'none'.  Examples of 
   * supplementary file types include original Affymetrix CEL and EXP 
   * files, GenePix GPR files, and TIFF image files. Supplementary files 
   * should be zipped or tarred together with the SOFT file at time of 
   * submission (do not include any sub-directories or sub-folders in 
   * your zip/tar archive). Provision of supplementary raw data files 
   * facilitates the unambiguous interpretation of data and potential 
   * verification of conclusions as set forth in the MIAME guidelines.
   ********************************************************************/
   
  std::string Sample_table;
  /* Label/Key         : !Sample_table
   * Number per file   : [0, 1]
   * Value constraints : ASCII, Tab-delimited table file , see notes.
   * 
   * Notes: Name of external CHP or tab-delimited file to be used as 
   * data table Affymetrix CHP file name:  If your processed data are 
   * CHP files, you can reference the CHP file name in this field. If 
   * your manuscript discusses data processed by RMA or another 
   * algorithm, we recommend providing those values in the table 
   * section. There is no need to specify the !Sample_platform_id when 
   * CHP files are supplied. All external files should be zipped or 
   * tarred together with the SOFT file at time of submission.
   * 
   * 
   * If it is convenient for you to generate, you can reference the name 
   * of an external tab-delimited table file (see format) in this field, 
   * rather than include the table in the !Sample_table_begin section. 
   * All external files should be zipped or tarred together with the 
   * SOFT file at time of submission.
   ********************************************************************/
  
  std::vector<std::string> Sample_source_name_ch;
  /* Label/Key         : !Sample_source_name_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII
   * 
   * Notes: Briefly identify the biological material and the 
   * experimental variable(s), e.g., vastus lateralis muscle, exercised, 
   * 60 min.
   ********************************************************************/
   
  std::vector<std::string> Sample_organism_ch;
  /* Label/Key         : !Sample_organism_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Use standard NCBI Taxonomy nomenclature Identify the 
   * organism(s) from which the biological material was derived.
   ********************************************************************/
   
  std::vector<std::string> Sample_characteristics_ch;
  /* Label/Key         : !Sample_characteristics_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, 'Tag: Value' format
   * 
   * Notes: Describe all available characteristics of the biological 
   * source, including factors not necessarily under investigation. 
   * Provide in 'Tag: Value' format, where 'Tag' is a type of 
   * characteristic (e.g. "gender", "strain", "tissue", "developmental 
   * stage", "tumor stage", etc), and 'Value' is the value for each tag 
   * (e.g. "female", "129SV", "brain", "embryo", etc). Include as many 
   * characteristics fields as necessary to thoroughly describe your 
   * Samples.
   ********************************************************************/
   
  std::vector<std::string> Sample_biomaterial_provider_ch;
  /* Label/Key         : !Sample_biomaterial_provider_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Specify the name of the company, laboratory or person that 
   * provided the biological material.
   ********************************************************************/
  
  std::vector<std::string> Sample_treatment_protocol;
  /* Label/Key         : !Sample_treatment_protocol_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe any treatments applied to the biological material 
   * prior to extract preparation. You can include as much text as you 
   * need to thoroughly describe the protocol; it is strongly 
   * recommended that complete protocol descriptions are provided within 
   * your submission.
   ********************************************************************/
   
  std::vector<std::string> Sample_growth_protocol_ch;
  /* Label/Key         : !Sample_growth_protocol_ch[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe the conditions that were used to grow or maintain 
   * organisms or cells prior to extract preparation. You can include as 
   * much text as you need to thoroughly describe the protocol; it is 
   * strongly recommended that complete protocol descriptions are 
   * provided within your submission.
   ********************************************************************/
   
  std::vector<std::string> Sample_molecule_ch;
  /* Label/Key         : !Sample_molecule_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII, Specify the type of molecule that was 
   *                     extracted from the biological material.
   * 
   * Notes:
   ********************************************************************/
   
  std::vector<std::string> Sample_extract_protocol_ch;
  /* Label/Key         : !Sample_extract_protocol_ch[n]
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe the protocol used to isolate the extract material. 
   * You can include as much text as you need to thoroughly describe the 
   * protocol; it is strongly recommended that complete protocol 
   * descriptions are provided within your submission.
   ********************************************************************/
   
  std::vector<std::string> Sample_label_ch;
  /* Label/Key         : !Sample_label_ch[n]
   * Number per file   : 1 per channel
   * Value constraints : ASCII, Specify the compound used to label the 
   *                     extract e.g., biotin, Cy3, Cy5, 33P.
   * 
   * Notes:
   ********************************************************************/
  
  std::vector<std::string> Sample_label_protocol_ch;
  /* Label/Key         : !Sample_label_protocol_ch[n]
   * Number per file   : [1, Infinty)
   * Value constraints : ASCII, extraction protocol description.
   * 
   * Notes: Describe the protocol used to label the extract. You can 
   * include as much text as you need to thoroughly describe the 
   * protocol; it is strongly recommended that complete protocol 
   * descriptions are provided within your submission.
   ********************************************************************/
   
  std::vector<std::string> Sample_hyb_protocol;
  /* Label/Key         : !Sample_hyb_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, describe hybridization protocol used.
   * 
   * Notes: Describe the protocols used for hybridization, blocking and 
   * washing, and any post-processing steps such as staining. You can 
   * include as much text as you need to thoroughly describe the 
   * protocol; it is strongly recommended that complete protocol 
   * descriptions are provided within your submission.
   ********************************************************************/
   
  std::vector<std::string> Sample_scan_protocol;
  /* Label/Key         : !Sample_scan_protocol
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, scanning/image acquisition protocols.
   * 
   * Notes: Describe the scanning and image acquisition protocols, 
   * hardware, and software. You can include as much text as you need to 
   * thoroughly describe the protocol; it is strongly recommended that 
   * complete protocol descriptions are provided within your submission.
   ********************************************************************/
   
  std::vector<std::string> Sample_data_processing;
  /* Label/Key         : !Sample_data_processing
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Provide details of how data in the VALUE column of your 
   * table were generated and calculated, i.e., normalization method, 
   * data selection procedures and parameters, transformation algorithm 
   * (e.g., MAS5.0), and scaling parameters. You can include as much 
   * text as you need to thoroughly describe the processing procedures.
   ********************************************************************/
   
  std::vector<std::string> Sample_description;
  /* Label/Key         : !Sample_description
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Include any additional information not provided in the other 
   * fields, or paste in broad descriptions that cannot be easily 
   * dissected into the other fields.
   ********************************************************************/
   
  std::string Sample_platform_id;
  /* Label/Key         : !Sample_platform_id
   * Number per file   : 1
   * Value constraints : ASCII, a valid Platform identifier.
   * 
   * Notes: Reference the Platform upon which this hybridization was 
   * performed.  Reference the Platform accession number (GPLxxx) if the 
   * Platform already exists in GEO, or reference the ^Platform 
   * identifier if the Platform record is being batch submitted within 
   * the same SOFT file.  To identify the accession number of an 
   * existing commercial Platform in GEO, use the FIND PLATFORM tool.
   ********************************************************************/
   
  std::string Sample_geo_accession;
  /* Label/Key         : !Sample_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Integer, valid Sample accession number.
   * 
   * Notes: a valid Sample accession number (GSMxxx).  Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
   
  std::string Sample_anchor;
  /* Label/Key         : !Sample_anchor
   * Number per file   : 1
   * Value constraints : See notes.
   * 
   * Notes: SAGE enzyme anchor, usually NlaIII or Sau3A   Use for SAGE 
   * submissions only.
   ********************************************************************/
   
  std::string Sample_type;
  /* Label/Key         : !Sample_type
   * Number per file   : 1
   * Value constraints : ASCII, SAGE.
   * 
   * Notes: SAGE   Use for SAGE submissions only.
   ********************************************************************/
  
  std::string Sample_tag_count;
  /* Label/Key         : !Sample_tag_count
   * Number per file   : 1
   * Value constraints : Number, sum of tags quantified in SAGE library.
   * 
   * Notes: Use for SAGE submissions only.
   ********************************************************************/
   
  std::string Sample_tag_length;
  /* Label/Key         : !Sample_tag_length
   * Number per file   : 1
   * Value constraints : Interger, base pair length of the SAGE tags, 
   *                     excluding anchor sequence.
   * 
   * Notes: Use for SAGE submissions only.
   ********************************************************************/
   
   //TODO: add doc, detail
   struct Sample_table sampleTable;
  /* Label/Key         : !Sample_table_begin
   * Number per file   : 
   * Value constraints : 
   * 
   * Notes:
   ********************************************************************/
  /* Label/Key         : !Sample_table_begin
   * Number per file   : 
   * Value constraints : 
   * 
   * Notes:
   ********************************************************************/
  
  //////////////////////////////////////////////////////////////////////
  
  std::string SERIES;
  /* Label/Key         : ^SERIES
   * Number per file   : 1
   * Value constraints : ASCII, unique in file.
   * 
   * Notes: Provide an identifier for this entity. This identifier is 
   * used only as an internal reference within a given file. The 
   * identifier will not appear on final GEO records.
   ********************************************************************/
   
  std::string Series_title;
  /* Label/Key         : !Series_title
   * Number per file   : 1
   * Value constraints : ASCII, [1, 255] characters, unique in file and 
   *                     by submitter.
   * 
   * Notes: Provide a unique title that describes the overall study.
   ********************************************************************/
   
  std::vector<std::string> Series_summary;
  /* Label/Key         : !Series_summary
   * Number per file   : [1, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Summarize the goals and objectives of this study. The 
   * abstract from the associated publication may be suitable. You can 
   * include as much text as you need to thoroughly describe the study.
   ********************************************************************/
   
  std::string Series_overall_design;
  /* Label/Key         : !Series_overall_design
   * Number per file   : 1
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Provide a description of the experimental design.  Indicate 
   * how many Samples are analyzed, if replicates are included, are 
   * there control and/or reference Samples, dye-swaps, etc.
   ********************************************************************/
   
  std::vector<std::string> Series_pubmed_id;
  /* Label/Key         : !Series_pubmed_id
   * Number per file   : [0, Infinity)
   * Value constraints : PubMed identifier
   * 
   * Notes: Specify a valid PubMed identifier (PMID) that references a 
   * published article describing this study. Most commonly, this 
   * information is not available at the time of submission - it can be 
   * added later once the data are published.
   ********************************************************************/
   
  std::vector<std::string> Series_web_link;
  /* Label/Key         : !Series_web_link
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, valid URL.
   * 
   * Notes: Specify a Web link that directs users to supplementary 
   * information about the study. Please restrict to Web sites that you 
   * know are stable.
   ********************************************************************/
   
  std::vector<std::string> Series_contributor;
  /* Label/Key         : !Series_contributor
   * Number per file   : [0, Infinity)
   * Value constraints : Contributor names, see notes.
   * 
   * Notes: List all people associated with this study.  Each value in 
   * the form 'firstname,middleinitial,lastname' or 
   * 'firstname,lastname': firstname must be at least one character and 
   * cannot contain spaces; middleinitial, if present, is one character; 
   * lastname is at least two characters and can contain spaces.   
   ********************************************************************/
   
  std::vector<std::string> Series_variable;
  /* Label/Key         : !Series_variable_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: Describe dose, time, tissue, strain, gender, cell line, 
   * development stage, age, agent, cell type, infection, isolate, 
   * metabolism, shock, stress, temperature, specimen, disease state, 
   * protocol, growth protocol, genotype/variation, species, individual, 
   * or other.  
   * Indicate the variable type(s) investigated in this study, e.g., 
   * !Series_variable_1 = age
   * !Series_variable_2 = age
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
   
  std::vector<std::string> Series_variable_description;
  /* Label/Key         : !Series_variable_description_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Describe each variable, e.g., 
   * !Series_variable_description_1 = 2 months
   * !Series_variable_description_2 = 12 months  
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
  
  std::vector<std::string> Series_variable_sample_list;
  /* Label/Key         : !Series_variable_sample_list_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : 
   * 
   * Notes: Each value a valid reference to a ^SAMPLE identifier, or all   
   * List which Samples belong to each group, e.g.,
   * !Series_variable_sample_list_1 = samA, samB
   * !Series_variable_sample_list_2 = samC, samD 
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
  
  //TODO: fix up doc
  std::vector<std::string> Series_repeats;
  /* Label/Key         : !Series_repeats_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII, see notes.
   * 
   * Notes: biological replicate, technical replicate - extract, or 
   * technical replicate - labeled-extract   Indicate the repeat 
   * type(s), e.g.,
   * !Series_repeats_1 = biological replicate
   * !Series_repeats_2 = biological replicate 
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
  
  //TODO: fix up doc
  std::vector<std::string> Series_repeats_sample_list;
  /* Label/Key         : !Series_repeats_sample_list_[n]
   * Number per file   : [0, Infinity)
   * Value constraints : ASCII
   * 
   * Notes: Each value a valid reference to a ^SAMPLE identifier, or all   
   * List which Samples belong to each group, e.g.,
   * !Series_repeats_sample_list_1 = samA, samB
   * !Series_repeats_sample_list_2 = samC, samD 
   * NOTE - this information is optional and does not appear in Series 
   * records or downloads, but will be used to assemble corresponding 
   * GEO DataSet records.
   ********************************************************************/
  
  std::vector<std::string> Series_sample_id;
  /* Label/Key         : !Series_sample_id
   * Number per file   : [1, Infinity)
   * Value constraints : Valid Sample identifiers
   * 
   * Notes: Reference the Samples that make up this experiment. 
   * Reference the Sample accession numbers (GSMxxx) if the Samples 
   * already exists in GEO, or reference the ^Sample identifiers if they 
   * are being submitted in the same file.
   ********************************************************************/
   
  std::string Series_geo_accession;
  /* Label/Key         : !Series_geo_accession
   * Number per file   : [0, 1]
   * Value constraints : Number
   * 
   * Notes: A valid Series accession number (GSExxx)   Only use for 
   * performing updates to existing GEO records.
   ********************************************************************/
  
};

typedef struct GEOSimpleFile simpleFile;

simpleFile getDefaultGEOSimpleFile();

int verifyGEOSimpleString(const simpleFile *toVerify);

int readGEOSimpleString(const char *contents, simpleFile &processed);

int writeGEOSimpleString(char **pth, const simpleFile &contents);

#endif